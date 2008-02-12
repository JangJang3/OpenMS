// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework 
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2008 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Marc Sturm $
// --------------------------------------------------------------------------

#ifndef OPENMS_DATASTRUCTURES_DPOSITION_H
#define OPENMS_DATASTRUCTURES_DPOSITION_H

#include <OpenMS/config.h>
#include <OpenMS/CONCEPT/Macros.h>

#include <algorithm>
#include <limits>

namespace OpenMS
{
	/**
		 @brief Representation of a coordinate in D-dimensional space.
		
		 @ingroup Datastructures		
	*/
	template <UInt D>
	class DPosition
	{
	 public:

		/// Coordinate type
		typedef DoubleReal CoordinateType;
		/// Mutable iterator
		typedef CoordinateType* Iterator;
		/// Non-mutable iterator
		typedef const CoordinateType*	ConstIterator;
		/// Dimensions
		enum { DIMENSION = D };
		/**	
			@name STL compatibility type definitions
		*/	
		//@{
		typedef	CoordinateType value_type;
		typedef CoordinateType& reference;
		typedef CoordinateType* pointer;
		typedef CoordinateType* iterator;
		typedef const CoordinateType* const_iterator;
		//@}
		
		/**	
			 @name Constructors and Destructor 
		*/
		//@{
		/** 
				@brief Default constructor.
			
				Creates a position with all coordinates zero.
		*/
		DPosition() { clear();	}
		
		/// Destructor (not-virtual as this will save a lot of space!)
		~DPosition() {}

		/// Constructor that fills all dimensions with the value @p x
		DPosition(CoordinateType x)
		{
			std::fill(&(coordinate_[0]), &(coordinate_[D]), x);
		}

		/// Copy constructor
		DPosition(const DPosition& pos)
		{
			std::copy(&(pos.coordinate_[0]), &(pos.coordinate_[D]), &(coordinate_[0]));
		}
		
		/// Constructor only for DPosition<2> that takes two Coordinates.
		DPosition(CoordinateType x, CoordinateType y)
		{
			OPENMS_PRECONDITION(D == 2, "DPosition<D>:DPosition(x,y): index overflow!");
			coordinate_[0]=x;
			coordinate_[1]=y;
		}
    
    /// Assignment operator
    DPosition& operator = (const DPosition& source)
    {
      if (&source==this)
        return *this;

      std::copy(&(source.coordinate_[0]), &(source.coordinate_[D]), &(coordinate_[0]));

      return *this;
    }

		//@}

		/**	@name Accessors */
		//@{
		
		///Const accessor for the dimensions
		CoordinateType operator [] (UInt index) const
		{
			OPENMS_PRECONDITION(index < D, "DPosition<D>:operator [] (Position): index overflow!");
			return coordinate_[index];
		}

		///Accessor for the dimensions
		CoordinateType& operator [] (UInt index)
		{
			OPENMS_PRECONDITION(index < D, "DPosition<D>:operator [] (Position): index overflow!");
			return coordinate_[index];
		}
		
		///Name accessor for the first dimension. Only for DPosition<2>, for visualization.
		CoordinateType getX() const
    {
    	OPENMS_PRECONDITION(D == 2, "DPosition<D>:getX(): index overflow!");
			return coordinate_[0];
    }
		
		///Name accessor for the second dimension. Only for DPosition<2>, for visualization.
		CoordinateType getY() const
    {
    	OPENMS_PRECONDITION(D == 2, "DPosition<D>:getY(): index overflow!");
			return coordinate_[1];
    }
		
		///Name mutator for the first dimension. Only for DPosition<2>, for visualization.
		void setX(CoordinateType c) 
    {
    	OPENMS_PRECONDITION(D == 2, "DPosition<D>:setX(): index overflow!");
			coordinate_[0] = c;
    }
		
		///Name mutator for the second dimension. Only for DPosition<2>, for visualization.
		void setY(CoordinateType c) 
    {
    	OPENMS_PRECONDITION(D == 2, "DPosition<D>:setY(): index overflow!");
			coordinate_[1] = c;
    }

		/// Equality operator
		bool operator == (const DPosition& point) const throw()
		{
			for (UInt i = 0; i < D; i++)
			{
				if (coordinate_[i] != point.coordinate_[i]) return false;
			}
			return true;
		}

		/// Equality operator
		bool operator != (const DPosition& point) const throw()
		{
			return !( operator==(point) );
		}

    /**
			 @brief Lexicographical less than operator.
    	
			 Lexicographical copmarison from dimension 0 to dimension D-1 is done.
		*/
		bool operator < (const DPosition& point) const throw()
		{
			for (UInt i = 0; i < D; i++)
			{
				if (coordinate_[i] < point.coordinate_[i]) return true;
				if (coordinate_[i] > point.coordinate_[i]) return false;
			}
			return false;
		}
		
		/// Lexicographical greater less or equal operator.	
		bool operator <= (const DPosition& point) const throw()
		{
			for (UInt i = 0; i < D; i++)
			{
				if (coordinate_[i] < point.coordinate_[i]) return true;
				if (coordinate_[i] > point.coordinate_[i]) return false;
			}
			return true;
		}
		
		/// Spatially (geometrically) less or equal operator.	 All coordinates must be "<=".
		bool spatiallyLessEqual(const DPosition& point) const throw()
		{
			for (UInt i = 0; i < D; i++)
			{
				if (coordinate_[i] > point.coordinate_[i]) return false;
			}
			return true;
		}
		
		/// Spatially (geometrically) greater or equal operator. All coordinates must be ">=".
		bool spatiallyGreaterEqual(const DPosition& point) const throw()
		{
			for (UInt i = 0; i < D; i++)
			{
				if (coordinate_[i] < point.coordinate_[i]) return false;
			}
			return true;
		}
		
    /// Lexicographical greater than operator.	
		bool operator > (const DPosition& point) const throw()
		{
			return !(operator<=(point));
		}
		
		/// Lexicographical greater or equal operator.	
		bool operator >= (const DPosition& point) const throw()
		{
			return !operator<(point);
		}

    /// Addition (a bit inefficient)
		DPosition operator + (const DPosition& point) const throw()
		{
      DPosition result(*this);
      for (UInt i = 0; i < D; result.coordinate_[i] += point.coordinate_[i], ++i);
      return result;
		}
		
    /// Addition
		DPosition & operator += (const DPosition& point) throw()
		{
      for (UInt i = 0; i < D; coordinate_[i] += point.coordinate_[i], ++i);
      return *this;
		}
		
		/// Subtraction (a bit inefficient)
		DPosition operator - (const DPosition& point) const throw()
		{
      DPosition result(*this);
      for (UInt i = 0; i < D; result.coordinate_[i] -= point.coordinate_[i], ++i);
      return result;
		}

    /// Subtraction
		DPosition & operator -= (const DPosition& point) throw()
		{
      for (UInt i = 0; i < D; coordinate_[i] -= point.coordinate_[i], ++i);
      return *this;
		}
		
    /// Negation (a bit inefficient)
    DPosition operator - () const throw()
    {
      DPosition<D> result(*this);
      for (UInt i=0; i < D; result.coordinate_[i] = -result.coordinate_[i] , ++i);
      return result;      
    }
          
		/// Inner product
		CoordinateType operator * (const DPosition& point) const throw()
		{
			CoordinateType prod(0);
			for (UInt i = 0; i < D; prod += (point.coordinate_[i] * coordinate_[i]), ++i);
			return prod;
		}
		
    /// Scalar multiplication
		DPosition & operator *= (CoordinateType scalar) throw()
		{
      for (UInt i = 0; i < D; coordinate_[i] *= scalar, ++i);
      return *this;
		}

    /// Scalar division
		DPosition & operator /= (CoordinateType scalar) throw()
		{
      for (UInt i = 0; i < D; coordinate_[i] /= scalar, ++i);
      return *this;
		}

		/// Returns the number of dimensions
		static UInt size() { return D; }
				
		/// Set all dimensions to zero
		void clear() { for (UInt i = 0; i < D; coordinate_[i++] = (CoordinateType)0); }
		//@}

		/**	@name Static default instances */
		//@{
		/// all zero
		static const DPosition zero;
		/// smallest positive
		static const DPosition min;
		/// smallest negative
		static const DPosition min_negative;
		/// largest positive
		static const DPosition max;
		//@}

		/**	@name Iteration */
		//@{
		/// Non-mutable begin iterator 
		ConstIterator begin() const throw() { return &(coordinate_[0]); }
		/// Non-mutable end iterator 
		ConstIterator end() const throw() { return &(coordinate_[0]) + D; }
		/// Mutable begin iterator 
		Iterator begin() throw() { return &(coordinate_[0]); }
		/// Mutable end iterator 
		Iterator end() throw() { return &(coordinate_[0]) + D; }
		//@}

	 protected:
		CoordinateType coordinate_[D];

	};  // DPosition

	/// Scalar multiplication (a bit inefficient)
	template <UInt D>
	DPosition<D> operator * (DPosition<D> position, typename DPosition<D>::CoordinateType scalar) throw()
	{
		for (UInt i = 0; i < D; position[i] *= scalar,++i) ;
		return position;
	}
	
	/// Scalar multiplication (a bit inefficient)
	template <UInt D>
	DPosition<D> operator * (typename DPosition<D>::CoordinateType scalar, DPosition<D> position) throw()
	{
		for (UInt i = 0; i < D; position[i] *= scalar,++i) ;
		return position;
	}

	template <UInt D>
	const DPosition<D> DPosition<D>::zero 
	= DPosition<D>(0);

	template <UInt D>
	const DPosition<D> DPosition<D>::min 
	= DPosition<D>(std::numeric_limits<typename DPosition<D>::CoordinateType>::min());

	template <UInt D>
	const DPosition<D> DPosition<D>::max 
	= DPosition<D>(std::numeric_limits<typename DPosition<D>::CoordinateType>::max());

	template <UInt D>
	const DPosition<D> DPosition<D>::min_negative
	= DPosition<D>(-std::numeric_limits<typename DPosition<D>::CoordinateType>::max());

	///Print the contents to a stream.
	template <UInt D>
	std::ostream& operator << (std::ostream& os, const DPosition<D>& pos)
	{
		os << pos[0];
		for (UInt i=1; i < D; ++i)
		{
			os << ' ' << pos[i];
		}
		
		return os;
	}

	
} // namespace OpenMS

#endif // OPENMS_DATASTRUCTURES_DPOSITION_H
