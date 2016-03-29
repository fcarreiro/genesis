#ifndef __3DMATH_H__
#define __3DMATH_H__

//////////////////////////////////////////////////////////////////////////
// General math constants
//////////////////////////////////////////////////////////////////////////

#define PI				3.1415926535897932
#define R_INTERSECTS	0
#define R_FRONT			1
#define R_BEHIND		2

//////////////////////////////////////////////////////////////////////////
// Vector() : Create vector from two points
//////////////////////////////////////////////////////////////////////////

inline CVector3 Vector(CVector3 & vPoint1, CVector3 & vPoint2)
{
	CVector3 vVector;

	// Substract coords
	vVector.x = vPoint1.x - vPoint2.x;
	vVector.y = vPoint1.y - vPoint2.y;
	vVector.z = vPoint1.z - vPoint2.z;

	// Return the resultant vector
	return vVector;
}

//////////////////////////////////////////////////////////////////////////
// Absolute() : Returns the absolute of "num"
//////////////////////////////////////////////////////////////////////////

inline float Absolute(float num)
{
	// return the absolute
	return (num >= 0 ? num : -num);
}

//////////////////////////////////////////////////////////////////////////
// Dot() : This returns the dot product of two vectors
//////////////////////////////////////////////////////////////////////////

inline float Dot(CVector3 & vVector1, CVector3 & vVector2)
{
	// The dot product is this equation: V1.V2 = (V1.x * V2.x  +  V1.y * V2.y  +  V1.z * V2.z)
	// In math terms, it looks like this:  V1.V2 = ||V1|| ||V2|| cos(theta)
	return ((vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z));
}

//////////////////////////////////////////////////////////////////////////
// Cross() : This returns a perpendicular vector (cross product)
//////////////////////////////////////////////////////////////////////////
												
inline CVector3 Cross(CVector3 & vVector1, CVector3 & vVector2)
{
	CVector3 vNormal;

	// The X value for the vector is:  (V1.y * V2.z) - (V1.z * V2.y)													// Get the X value
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
														
	// The Y value for the vector is:  (V1.z * V2.x) - (V1.x * V2.z)
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
														
	// The Z value for the vector is:  (V1.x * V2.y) - (V1.y * V2.x)
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// Return the cross product (Direction the polygon is facing - Normal)
	return vNormal;
}

//////////////////////////////////////////////////////////////////////////
// Magnitude() : This returns the magnitude of a normal (or any other vector)
//////////////////////////////////////////////////////////////////////////

inline float Magnitude(CVector3 & vNormal)
{
	// magnitude = sqrt(V.x^2 + V.y^2 + V.z^2)
	return (float) sqrt((vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z));
}

//////////////////////////////////////////////////////////////////////////
// Normalize() : This returns a normalized vector (A vector exactly of length 1)
//////////////////////////////////////////////////////////////////////////

inline CVector3 Normalize(CVector3 & vNormal)
{
	// Return our normalized vector
	return (vNormal / Magnitude(vNormal));
}

//////////////////////////////////////////////////////////////////////////
// Normal() : This returns the normal of a polygon
//////////////////////////////////////////////////////////////////////////

inline CVector3 Normal(CVector3 vPolygon[])					
{
	CVector3 vNormal;

	// Get the two vectors from the polygon
	CVector3 vVector1 = vPolygon[2] - vPolygon[0];
	CVector3 vVector2 = vPolygon[1] - vPolygon[0];

	// Take the cross product of our 2 vectors to get a perpendicular vector
	vNormal = Cross(vVector1, vVector2);

	// Normalize it
	vNormal = Normalize(vNormal);

	// return the normal
	return vNormal;
}

//////////////////////////////////////////////////////////////////////////
// Distance() : This returns the distance between two 3D points
//////////////////////////////////////////////////////////////////////////

inline float Distance(CVector3 & vPoint1, CVector3 & vPoint2)
{
	// distance = sqrt((P2.x - P1.x)^2 + (P2.y - P1.y)^2 + (P2.z - P1.z)^2)
	float distance = (float) sqrt((vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
						          (vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
						          (vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z));

	// Return the distance between the 2 points
	return distance;
}

//////////////////////////////////////////////////////////////////////////
// Distance() : This returns the distance between 2 3D points
//////////////////////////////////////////////////////////////////////////

inline CVector3 ClosestPointOnLine(CVector3 & vA, CVector3 & vB, CVector3 & vPoint)
{
	// Create the vector from end point vA to our point vPoint.
	CVector3 vVector1 = vPoint - vA;

	// Create a normalized direction vector from end point vA to end point vB
    CVector3 vVector2 = Normalize(vB - vA);

	// Use the distance formula to find the distance of the line segment (or magnitude)
    float d = Distance(vA, vB);

	// Using the dot product, we project the vVector1 onto the vector vVector2.
	// This essentially gives us the distance from our projected vector from vA.
    float t = Dot(vVector2, vVector1);

	// If our projected distance from vA, "t", is less than or equal to 0, it must
	// be closest to the end point vA.  We want to return this end point.
    if (t <= 0) return vA;

	// If our projected distance from vA, "t", is greater than or equal to the magnitude
	// or distance of the line segment, it must be closest to the end point vB.
    if (t >= d) return vB;
 
	// Here we create a vector that is of length t and in the direction of vVector2
    CVector3 vVector3 = vVector2 * t;

	// To find the closest point on the line segment, we just add vVector3 to the original
	// end point vA.  
    CVector3 vClosestPoint = vA + vVector3;

	// Return the closest point on the line segment
	return vClosestPoint;
}

//////////////////////////////////////////////////////////////////////////
// PlaneDistance() : This returns the distance between a plane and the origin
//////////////////////////////////////////////////////////////////////////
								
inline float PlaneDistance(CVector3 & Normal, CVector3 & Point)
{	
	// Use the plane equation to find the distance (Ax + By + Cz + D = 0)
	// So, we come up with D = -(Ax + By + Cz)
	return -((Normal.x * Point.x) + (Normal.y * Point.y) + (Normal.z * Point.z));
}

//////////////////////////////////////////////////////////////////////////
// IntersectedPlane() : This checks to see if a line R_INTERSECTS a plane
//////////////////////////////////////////////////////////////////////////
											
inline bool IntersectedPlane(CVector3 vPoly[], CVector3 vLine[], CVector3 & vNormal, float & originDistance)
{
	float distance1;
	float distance2;

	// We need to get the normal of our plane to go any further	
	vNormal = Normal(vPoly);

	// Let's find the distance our plane is from the origin.  We can find this value
	// from the normal to the plane (polygon) and any point that lies on that plane
	originDistance = PlaneDistance(vNormal, vPoly[0]);

	// Get the distance from point1 from the plane
	distance1 = ((vNormal.x * vLine[0].x)  +
		         (vNormal.y * vLine[0].y)  +
				 (vNormal.z * vLine[0].z)) + originDistance;
	
	// Get the distance from point2 from the plane
	distance2 = ((vNormal.x * vLine[1].x)  +
		         (vNormal.y * vLine[1].y)  +
				 (vNormal.z * vLine[1].z)) + originDistance;

	// Now that we have 2 distances from the plane, if we times them together we either
	// get a positive or negative number.  If it's a negative number, that means we collided!
	// This is because the 2 points must be on either side of the plane (IE. -1 * 1 = -1).
	// Check to see if both point's distances are both negative or both positive
	// Return false if each point has the same sign.
	// -1 and 1 would mean each point is on either side of the plane.
	if(distance1 * distance2 >= 0) return false;

	// The line intersected the plane, Return TRUE					
	return true;
}

//////////////////////////////////////////////////////////////////////////
// AngleBetweenVectors() : This checks to see if a point is inside the ranges of a polygon
//////////////////////////////////////////////////////////////////////////

inline double AngleBetweenVectors(CVector3 & Vector1, CVector3 & Vector2)
{							
	// Get the dot product of the vectors
	float dotProduct = Dot(Vector1, Vector2);

	// Get the product of both of the vectors magnitudes
	float vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2);

	// Get the angle in radians between the 2 vectors
	double angle = acos(dotProduct / vectorsMagnitude);

	// Here we make sure that the angle is not a -1.#IND0000000 which means indefinate
	if(_isnan(angle)) return 0;
	
	// Return the angle in radians
	return angle;
}

//////////////////////////////////////////////////////////////////////////
// IntersectionPoint() : This returns the intersection point of the line
//////////////////////////////////////////////////////////////////////////

inline CVector3 IntersectionPoint(CVector3 & vNormal, CVector3 vLine[], double distance)
{
	CVector3 vPoint, vLineDir;
	double Numerator = 0.0, Denominator = 0.0, dist = 0.0;

	// 1)  First we need to get the vector of our line, Then normalize it
	vLineDir = vLine[1] - vLine[0];
	vLineDir = Normalize(vLineDir);

	// 2) Use the plane equation (distance = Ax + By + Cz + D) to find the 
	// distance from one of our points to the plane.
	Numerator = -(vNormal.x * vLine[0].x + vNormal.y * vLine[0].y + vNormal.z * vLine[0].z + distance);

	// 3) If we take the dot product between our line vector and the normal of the polygon,
	Denominator = Dot(vNormal, vLineDir);

	// Since we are using division, we need to make sure we don't get a divide by zero error
	// If we do get a 0, that means that there are INFINATE points because the the line is
	// on the plane (the normal is perpendicular to the line - (Normal.Vector = 0)).  
	// In this case, we should just return any point on the line.
	if(Denominator == 0) return vLine[0];

	// Divide to get the multiplying (percentage) factor
	dist = Numerator / Denominator;
	
	// Now, like we said above, we times the dist by the vector, then add our arbitrary point.
	vPoint.x = (float)(vLine[0].x + (vLineDir.x * dist));
	vPoint.y = (float)(vLine[0].y + (vLineDir.y * dist));
	vPoint.z = (float)(vLine[0].z + (vLineDir.z * dist));

	// Return the intersection point
	return vPoint;
}

//////////////////////////////////////////////////////////////////////////
// InsidePolygon() : This checks to see if a point is inside the ranges of a polygon
//////////////////////////////////////////////////////////////////////////

inline bool InsidePolygon(CVector3 & vIntersection, CVector3 Poly[], long verticeCount)
{
	double Angle = 0.0;						// Initialize the angle
	CVector3 vA, vB;						// Create temp vectors
	const double MATCH_FACTOR = 0.99;		// Used to cover up the error in floating point
	
	// Go in a circle to each vertex and get the angle between
	for (int i = 0; i < verticeCount; ++i)
	{	
		// Subtract the intersection point from the current vertex
		vA = Poly[i] - vIntersection;
		
		// Subtract the point from the next vertex
		vB = Poly[(i + 1) % verticeCount] - vIntersection;
											
		// Find the angle between the 2 vectors and add them all up as we go along
		Angle += AngleBetweenVectors(vA, vB);
	}

	// If the angle is greater than 2 PI, (360 degrees) the point is inside
	if(Angle >= (MATCH_FACTOR * (2.0 * PI))) return true;

	// If you get here, it obviously wasn't inside the polygon, so Return FALSE
	return false;
}

//////////////////////////////////////////////////////////////////////////
// IntersectedPolygon() : This checks if a line is intersecting a polygon
//////////////////////////////////////////////////////////////////////////

inline bool IntersectedPolygon(CVector3 vPoly[], CVector3 vLine[], int verticeCount)
{
	CVector3 vNormal;
	float originDistance = 0;

	// First, make sure our line R_INTERSECTS the plane
	if(!IntersectedPlane(vPoly, vLine, vNormal, originDistance)) return false;

	// Now that we have our normal and distance passed back from IntersectedPlane(), 
	// we can use it to calculate the intersection point.  
	CVector3 vIntersection = IntersectionPoint(vNormal, vLine, originDistance);

	// Now that we have the intersection point, we need to test if it's inside the polygon.
	if(InsidePolygon(vIntersection, vPoly, verticeCount)) return true;

	// There was no collision, so return false
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ClassifySphere() : Where is the sphere
//////////////////////////////////////////////////////////////////////////

inline int ClassifySphere(CVector3 & vCenter, CVector3 & vNormal, CVector3 & vPoint, float radius, float *distance)
{
	// First we need to find the distance our polygon plane is from the origin.
	float d = PlaneDistance(vNormal, vPoint);

	// Here we use the famous distance formula to find the distance the center point
	// of the sphere is from the polygon's plane.  
	float dist = (vNormal.x * vCenter.x + vNormal.y * vCenter.y + vNormal.z * vCenter.z + d);

	// If we request the distance, set it
	if(distance) *distance = dist;

	// If the absolute value of the distance we just found is less than the radius, 
	// the sphere intersected the plane.
	// Else, if the distance is greater than or equal to the radius, the sphere is
	// completely in R_FRONT of the plane.
	if(Absolute(dist) < radius)	return R_INTERSECTS;
	else if(dist >= radius)	return R_FRONT;
	
	// If the sphere isn't intersecting or in R_FRONT of the plane, it must be R_BEHIND
	return R_BEHIND;
}

//////////////////////////////////////////////////////////////////////////
// EdgeSphereCollision() : Is the sphere intersecting any edge
//////////////////////////////////////////////////////////////////////////

inline bool EdgeSphereCollision(CVector3 & vCenter, CVector3 vPolygon[], int vertexCount, float radius)
{
	CVector3 vPoint;

	// Go through all of the vertices in the polygon
	for(int i = 0; i < vertexCount; ++i)
	{
		// This returns the closest point on the current edge to the center of the sphere.
		vPoint = ClosestPointOnLine(vPolygon[i], vPolygon[(i + 1) % vertexCount], vCenter);
		
		// Now, we want to calculate the distance between the closest point and the center
		float distance = Distance(vPoint, vCenter);

		// If the distance is less than the radius, there must be a collision so return true
		if(distance < radius) return true;
	}

	// The was no intersection of the sphere and the edges of the polygon
	return false;
}

//////////////////////////////////////////////////////////////////////////
// SpherePolygonCollision() : True if our sphere collides with the polygon passed in
//////////////////////////////////////////////////////////////////////////

inline bool SpherePolygonCollision(CVector3 vPolygon[], CVector3 & vCenter, int vertexCount, float radius)
{
	// Let's use our Normal() function to return us the normal to this polygon
	CVector3 vNormal = Normal(vPolygon);

	// This will store the distance our sphere is from the plane
	float distance = 0.0f;

	// This is where we determine the sphere
	int classification = ClassifySphere(vCenter, vNormal, vPolygon[0], radius, &distance);

	// If the sphere R_INTERSECTS the polygon's plane, then we need to check further
	if(classification == R_INTERSECTS) 
	{
		// 2) STEP TWO - Finding the psuedo intersection point on the plane
		// Now we want to project the sphere's center onto the polygon's plane
		CVector3 vOffset = vNormal * distance;

		// Once we have the offset to the plane, we just subtract it from the center
		// of the sphere. "vPosition" now a point that lies on the plane of the polygon.
		CVector3 vPosition = vCenter - vOffset;

		// 3) STEP THREE - Check if the intersection point is inside the polygons perimeter
		// If the intersection point is inside the perimeter of the polygon, it returns true.
		if(InsidePolygon(vPosition, vPolygon, 3))
		{
			// We collided!
			return true;
		}
		else
		{
			// 4) STEP FOUR - Check the sphere R_INTERSECTS any of the polygon's edges
			// If we get here, we didn't find an intersection point in the perimeter.
			// We now need to check collision against the edges of the polygon.
			if(EdgeSphereCollision(vCenter, vPolygon, vertexCount, radius))
			{
				// We collided!
				return true;
			}
		}
	}

	// If we get here, there is obviously no collision
	return false;
}

//////////////////////////////////////////////////////////////////////////
// GetCollisionOffset() : The offset to move the center of the sphere off the collided polygon
//////////////////////////////////////////////////////////////////////////

inline CVector3 GetCollisionOffset(CVector3 & vNormal, float radius, float distance)
{
	CVector3 vOffset;

	// Once we find if a collision has taken place, we need make sure the sphere
	// doesn't move into the wall.  In our app, the position will actually move into
	// the wall, but we check our collision detection before we render the scene, which
	// eliminates the bounce back effect it would cause.  The question is, how do we
	// know which direction to move the sphere back?  In our collision detection, we
	// account for collisions on both sides of the polygon.  Usually, you just need
	// to worry about the side with the normal vector and positive distance.  If 
	// you don't want to back face cull and have 2 sided planes, I check for both sides.
	//
	// Let me explain the math that is going on here.  First, we have the normal to
	// the plane, the radius of the sphere, as well as the distance the center of the
	// sphere is from the plane.  In the case of the sphere colliding in the R_FRONT of
	// the polygon, we can just subtract the distance from the radius, then multiply
	// that new distance by the normal of the plane.  This projects that leftover
	// distance along the normal vector.  For instance, say we have these values:
	//
	//	vNormal = (1, 0, 0)		radius = 5		distance = 3
	//
	// If we subtract the distance from the radius we get: (5 - 3 = 2)
	// The number 2 tells us that our sphere is over the plane by a distance of 2.
	// So basically, we need to move the sphere back 2 units.  How do we know which
	// direction though?  This part is easy, we have a normal vector that tells us the
	// direction of the plane.  
	// If we multiply the normal by the left over distance we get:  (2, 0, 0)
	// This new offset vectors tells us which direction and how much to move back.
	// We then subtract this offset from the sphere's position, giving is the new
	// position that is lying right on top of the plane.  Ba da bing!
	// If we are colliding from R_BEHIND the polygon (not usual), we do the opposite
	// signs as seen below:
	
	// If our distance is greater than zero, we are in R_FRONT of the polygon
	if(distance > 0)
	{
		// Find the distance that our sphere is overlapping the plane, then
		// find the direction vector to move our sphere.
		float distanceOver = radius - distance;
		vOffset = vNormal * distanceOver;
	}
	// Else colliding from R_BEHIND the polygon
	else
	{
		// Find the distance that our sphere is overlapping the plane, then
		// find the direction vector to move our sphere.
		float distanceOver = radius + distance;
		vOffset = vNormal * -distanceOver;
	}

	// There is one problem with check for collisions R_BEHIND the polygon, and that
	// is if you are moving really fast and your center goes past the R_FRONT of the
	// polygon, it will then assume you were colliding from R_BEHIND and not let
	// you back in.  Most likely you will take out the if / else check, but I
	// figured I would show both ways in case someone didn't want to back face cull.

	// Return the offset we need to move back to not be intersecting the polygon.
	return vOffset;
}

//////////////////////////////////////////////////////////////////////////
// End
//////////////////////////////////////////////////////////////////////////

#endif
