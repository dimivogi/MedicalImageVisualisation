#include	<iostream>
#include	<cstdarg>
using namespace std;



#ifndef VECTOR_TEMPLATE
#define VECTOR_TEMPLATE


// A class that represents a (multidimensional) vector
template < typename type = float , int size = 2 >
class Vector
{

	private:

		// Vector Dimension
		int							dimension;
		// Vector Data
		type						e[size];



	public:

		Vector();
		Vector( int params , ... );
		Vector( const Vector<type,size>& vec );
		~Vector();

		// Operator Overloading
		template < typename type , int size > friend istream&	operator>> ( istream& is , Vector<type,size>& vec );
		template < typename type , int size > friend ostream&	operator<< ( ostream& os , const Vector<type,size>& vec );

		Vector<type,size>&			operator= ( const Vector<type,size>& vec);
		Vector<type,size>			operator+ (); // unary
		Vector<type,size>			operator- ();

		Vector<type,size>			operator+ ( const type value );
		Vector<type,size>			operator+ ( const Vector<type,size>& vec );
		Vector<type,size>			operator- ( const type value );
		Vector<type,size>			operator- ( const Vector<type,size>& vec );
		Vector<type,size>			operator* ( const type value ); // scalar
		Vector<type,size>			operator* ( const Vector<type,size>& vec ); // dot product
		Vector<type,size>			operator/ ( const type value ); // scalar product with 1/value

		Vector<type,size>&			operator+= ( const type value );
		Vector<type,size>&			operator+= ( const Vector<type,size>& vec );
		Vector<type,size>&			operator-= ( const type value );
		Vector<type,size>&			operator-= ( const Vector<type,size>& vec );
		Vector<type,size>&			operator*= ( const type value ); // scalar product
		Vector<type,size>&			operator*= ( const Vector<type,size>& vec ); // dot product
		Vector<type,size>&			operator/= ( const type value ); // scalar product with 1/value

		bool						operator== ( const Vector<type,size>& vec ) const;
		bool						operator!= ( const Vector<type,size>& vec ) const;
		bool						operator>= ( const Vector<type,size>& vec ) const;
		bool						operator<= ( const Vector<type,size>& vec ) const;
		bool						operator> ( const Vector<type,size>& vec ) const;
		bool						operator< ( const Vector<type,size>& vec ) const;

		// Cross Product of this vector with the given Vector
		Vector<type,size>			cross( const Vector<type,size>& vec );
		// The normalized vector produced by this vector
		Vector<type,size>			normalize();

		// The length of this vector
		type						length() const;

		// Get the desired element of this vector
		type						getElement( int el ) const;
		// Set the desired element of this vector
		void						setElement( int el , type value );
};

// Boolean vectors are not supported
template < int size >
class Vector<bool,size>	{};


// Macros for easier access to the first 3 elements of a vector
#define	x()	getElement(0)
#define	r()	x()
#define	s()	x()

#define	y()	getElement(1)
#define	g()	y()
#define	t()	y()

#define	z()	getElement(2)
#define	b()	z()
#define	p()	z()

#define	w()	getElement(3)
#define	a()	w()
#define	q()	w()

#define	X(value)	setElement(0,value)
#define	R(value)	X(value)
#define	S(value)	X(value)

#define	Y(value)	setElement(1,value)
#define	G(value)	Y(value)
#define	T(value)	Y(value)

#define	Z(value)	setElement(2,value)
#define	B(value)	Z(value)
#define	P(value)	Z(value)

#define	W(value)	setElement(3,value)
#define	A(value)	W(value)
#define	Q(value)	W(value)


template < typename type , int size >
type			Vector<type,size>::getElement( int el ) const
{
	if ( el >=0  &&  el < dimension )
		return e[el];
	else
		return (type)0;
};

template < typename type , int size >
void			Vector<type,size>::setElement( int el , type value )
{
	if ( el >=0  &&  el < dimension )
		e[el] = value;
};


template < typename type , int size >
Vector<type,size>::Vector() : dimension(size)
{
	for ( int i = 0;  i < dimension;  i++ )
		e[i] = (type)0;
};

template < typename type , int size >
Vector<type,size>::Vector( int params , ... ) : dimension(size)
{
	int i,j;
	long double val;

	va_list vl;
	va_start(vl,params);
	
	i = 0;
	j = 0;
	while ( i < dimension  &&  j < params )
	{
		val=va_arg(vl,long double);
		e[i] = (type)val;

		i++;
		j++;
	}
	  va_end(vl);

	for ( int k = i;  k < dimension;  k++ )
		e[k] = 0;
};

template < typename type , int size >
Vector<type,size>::Vector( const Vector<type,size>& vec ) : dimension(size)
{
	for ( int i = 0;  i < dimension;  i++ )
		e[i] = vec.getElement(i);
};

template < typename type , int size >
Vector<type,size>::~Vector()	{};


template < typename type , int size >
istream&	operator>> ( istream& is , Vector<type,size>& vec )
{
	for ( int i = 0;  i < vec.dimension;  i++)
		is >> vec.e[i];

	return is;
};

template < typename type , int size >
ostream&	operator<< ( ostream& os , const Vector<type,size>& vec )
{
	os << "(";
	for ( int i = 0;  i < (vec.dimension-1);  i++)
		os << vec.e[i] << ", ";
	os << vec.e[vec.dimension-1] << ")";

	return os;
};


template < typename type , int size >
Vector<type,size>&			Vector<type,size>::operator= ( const Vector<type,size>& vec)
{
	for ( int i = 0;  i < dimension;  i++ )
		e[i] = vec.getElement(i);

	return *this;
};

template < typename type , int size >
Vector<type,size>			Vector<type,size>::operator+ ()
{
	return *this;
};

template < typename type , int size >
Vector<type,size>			Vector<type,size>::operator- ()
{
	Vector<type,size>	returnValue;

	for ( int i = 0;  i < dimension;  i++ )
		returnValue.setElement(i,-e[i]);

	return returnValue;
};

template < typename type , int size >
Vector<type,size>			Vector<type,size>::operator+ ( const type value )
{
	Vector<type,size>	returnValue;

	for ( int i = 0;  i < dimension;  i++ )
		returnValue.setElement(i,e[i]+value);

	return returnValue;
};

template < typename type , int size >
Vector<type,size>			Vector<type,size>::operator+ ( const Vector<type,size>& vec )
{
	Vector<type,size>	returnValue;

	for ( int i = 0;  i < dimension;  i++ )
		returnValue.setElement(i,e[i]+vec.getElement(i));

	return returnValue;
};

template < typename type , int size >
Vector<type,size>			Vector<type,size>::operator- ( const type value )
{
	Vector<type,size>	returnValue;

	for ( int i = 0;  i < dimension;  i++ )
		returnValue.setElement(i,e[i]-value);

	return returnValue;
};

template < typename type , int size >
Vector<type,size>			Vector<type,size>::operator- ( const Vector<type,size>& vec )
{
	Vector<type,size>	returnValue;

	for ( int i = 0;  i < dimension;  i++ )
		returnValue.setElement(i,e[i]-vec.getElement(i));

	return returnValue;
};

template < typename type , int size >
Vector<type,size>			Vector<type,size>::operator* ( const type value )
{
	Vector<type,size>	returnValue;

	for ( int i = 0;  i < dimension;  i++ )
		returnValue.setElement(i,e[i]*value);

	return returnValue;
}; // scalar

template < typename type , int size >
Vector<type,size>			Vector<type,size>::operator* ( const Vector<type,size>& vec )
{
	Vector<type,size>	returnValue;

	for ( int i = 0;  i < dimension;  i++ )
		returnValue.setElement(i,e[i]*vec.getElement(i));

	return returnValue;
}; // dot

template < typename type , int size >
Vector<type,size>			Vector<type,size>::operator/ ( const type value )
{
	Vector<type,size>	returnValue;

	for ( int i = 0;  i < dimension;  i++ )
		returnValue.setElement(i,e[i]/value);

	return returnValue;
}; // scalar


template < typename type , int size >
Vector<type,size>&			Vector<type,size>::operator+= ( const type value )
{
	for ( int i = 0;  i < dimension;  i++ )
		e[i] += value;

	return *this;
};

template < typename type , int size >
Vector<type,size>&			Vector<type,size>::operator+= ( const Vector<type,size>& vec )
{
	for ( int i = 0;  i < dimension;  i++ )
		e[i] += vec.getElement(i);

	return *this;
};

template < typename type , int size >
Vector<type,size>&			Vector<type,size>::operator-= ( const type value )
{
	for ( int i = 0;  i < dimension;  i++ )
		e[i] -= value;

	return *this;
};

template < typename type , int size >
Vector<type,size>&			Vector<type,size>::operator-= ( const Vector<type,size>& vec )
{
	for ( int i = 0;  i < dimension;  i++ )
		e[i] -= vec.getElement(i);

	return *this;
};

template < typename type , int size >
Vector<type,size>&			Vector<type,size>::operator*= ( const type value )
{
	for ( int i = 0;  i < dimension;  i++ )
		e[i] *= value;

	return *this;
}; // scalar

template < typename type , int size >
Vector<type,size>&			Vector<type,size>::operator*= ( const Vector<type,size>& vec )
{
	for ( int i = 0;  i < dimension;  i++ )
		e[i] *= vec.getElement(i);

	return *this;
}; // dot

template < typename type , int size >
Vector<type,size>&			Vector<type,size>::operator/= ( const type value )
{
	for ( int i = 0;  i < dimension;  i++ )
		e[i] /= value;

	return *this;
}; // scalar


template < typename type , int size >
bool			Vector<type,size>::operator== ( const Vector<type,size>& vec ) const
{
	bool	returnValue = true;


	int i = 0;
	while ( i < dimension  &&  returnValue )
	{
		if ( e[i] != vec.getElement(i) )
			returnValue = false;
		else
			i++;
	}

	return returnValue;
};

template < typename type , int size >
bool			Vector<type,size>::operator!= ( const Vector<type,size>& vec ) const
{
	bool	returnValue = false;


	int i = 0;
	while ( i < dimension  &&  !returnValue )
	{
		if ( e[i] != vec.getElement(i) )
			returnValue = true;
		else
			i++;
	}

	return returnValue;
};

template < typename type , int size >
bool			Vector<type,size>::operator>= ( const Vector<type,size>& vec ) const
{
	if ( this->length() >= vec.length() )
		return true;
	else
		return false;
};

template < typename type , int size >
bool			Vector<type,size>::operator<= ( const Vector<type,size>& vec ) const
{
	if ( this->length() <= vec.length() )
		return true;
	else
		return false;
};

template < typename type , int size >
bool			Vector<type,size>::operator> ( const Vector<type,size>& vec ) const
{
	if ( this->length() > vec.length() )
		return true;
	else
		return false;
};

template < typename type , int size >
bool			Vector<type,size>::operator< ( const Vector<type,size>& vec ) const
{
	if ( this->length() < vec.length() )
		return true;
	else
		return false;
};


template < typename type , int size >
Vector<type,size>			Vector<type,size>::cross( const Vector<type,size>& vec )
{
	Vector<type,size>	returnValue;

	for ( int i = 0;  i < dimension;  i++ )
		returnValue.setElement(i,e[i]-vec.getElement(i));

	return returnValue;
};

template < typename type , int size >
Vector<type,size>			Vector<type,size>::normalize()
{
	Vector<type,size>	returnValue;

	returnValue = *this/this->length();

	return returnValue;
};

template < typename type , int size >
type			Vector<type,size>::length() const
{
	type	total = (type)0;


	for ( int i = 0;  i < dimension;  i++ )
		total += e[i]*e[i];

	return sqrt(total);
};


#endif	/* VECTOR_TEMPLATE */