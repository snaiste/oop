### Description
A class for vectors.

### Method and Structure
* Code defines a general vector class, that implements a Cartesian vector of length n
* The class contains a default constructor, parametrized constructor and destructor, copy constructor and copy assignment operator, move constructor and move assignment
* It also contains an accessor returning components of a vector (possible to use operator() or operator[], the latter is used in this code); code also checks for addressing outside the length of the vector
* There is a function to calculate the dot product of two vectors of identical length (code checks for length)
* There is a friend function for inserting vectors into an ostream
* There is a second class for Minkowski 4-vectors that is a specialisation of the vector class:
	* r1 = (x1, y1, z1, c t1);
	* r1 ⋅ r2 = c^2 t1 t2 − x1 x2 − y1 y2 − z1 z2 = c^2 t1 t2 − r1 ⋅ r2 and the Lorentz transform ("boost"):
	* c t' = γ (c t − β ⋅ x)
	* r' = r + ((γ − 1) r ⋅ β / β^2 − γ c t) β
	* γ = 1 / sqrt(1 − β^2)
* The derived class contains at least two parametrized constructors, the first taking four doubles (c t, x, y, z) and the second taking one double (c t) and one length 3-vector (r) as arguments
* It also has a copy constructor and copy assignment operator, move constructor and move assignment accessors returning 4-vector components
* It also has an overridden function to compute the dot product of 4-vectors
* A function to return a Lorentz-boosted 4-vector (needs the 3-vector β = v / c as sole parameter)
* A friend function for inserting 4-vectors into cout
* Threre is a third class (particle) that contains a 4-vector for the current position, a double mass and 3-vector β as a data members
* The class contains: a parameterized constructor (taking a 4-vector, a double and a 3-vector: r, m, β ); a function to calculate γ; a function to return the total energy of the particle
* There is a function to return the momentum of the particle
* Mass and momentum are expressed in energy units (e.g. MeV/c2, MeV/c).