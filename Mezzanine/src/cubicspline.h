// © Copyright 2010 - 2014 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _cubicspline_h
#define _cubicspline_h

/// @file
/// @brief

#include "datatypes.h"

namespace Mezzanine
{
    // The following license applies only to the Element and Spline class included below.
    /* "THE BEER-WARE LICENSE" (Revision 42): Devin Lane wrote this file. As long as you retain
     * this notice you can do whatever you want with this stuff. If we meet some day, and you
     * think this stuff is worth it, you can buy me a beer in return. */
    /// @brief Meta data nodes required to generate a cubic spline
    template <typename X, typename Y>
    class CubicSplineElement
    {
        public:

            /// @brief Time series meta-data
            X x;

            /// @brief Meta-data corresponding to "a" on wikipedia description of a cubic splines
            Y a;
            /// @brief Meta-data corresponding to "b" on wikipedia description of a cubic splines
            Y b;
            /// @brief Meta-data corresponding to "c" on wikipedia description of a cubic splines
            Y c;
            /// @brief Meta-data corresponding to "d" on wikipedia description of a cubic splines
            Y d;

            /// @brief Simple constructor
            /// @param _x
            CubicSplineElement(X _x) : x(_x) {}

            /// @brief Complete meta-data constructor
            /// _x Time series location
            /// _a  Meta-data corresponding to "a" on wikipedia description of a cubic splines
            /// _b  Meta-data corresponding to "b" on wikipedia description of a cubic splines
            /// _c  Meta-data corresponding to "c" on wikipedia description of a cubic splines
            /// _d  Meta-data corresponding to "d" on wikipedia description of a cubic splines
            CubicSplineElement(X _x, Y _a, Y _b, Y _c, Y _d)
                : x(_x), a(_a), b(_b), c(_c), d(_d)
            {}

            /// @brief The actual interpolation of the meta-data into a result
            /// @param xx Location in the time series toget the corresponding Data series location on a cubic curve.
            Y eval(const X& xx) const
            {
                X xix(xx - x);
                return a + b * xix + c * (xix * xix) + d * (xix * xix * xix);
            }

            /// @brief Sort Meta-data elements by time series locations in other Meta-Data
            /// @param e The CubicSplineElement to compare this one too.
            Boolean operator<(const CubicSplineElement& e) const
                { return x < e.x; }

            /// @brief Sort Meta-data elements by time series locations
            /// @param xx The CubicSplineElement to compare this one too.
            Boolean operator<(const X& xx) const
                { return x < xx; }
    };

    /// @brief A class for interpolating data with arbitrary
    /// @details Templated on type of X, Y. X and Y must have operator +, -, *, /. Y must have defined
    /// a constructor that takes a scalar.
    template <typename TimeSeriesType, typename DataSeriesType>
    class CubicSpline
    {
        public:
            /// @brief The meta data type stored by this container.
            typedef CubicSplineElement<TimeSeriesType,DataSeriesType> element_type;
            /// @brief The underlying type of the container that actually stores the instances of ContainerType.
            typedef std::vector<element_type> ContainerType;
            /// @brief An iterator suitable for the metadata this spline uses.
            typedef typename ContainerType::iterator iterator;
            /// @brief An iterator suitable for the metadata this spline uses that will not allow changes to the underlying data.
            typedef typename ContainerType::const_iterator const_iterator;

            /// @brief The actual container of Metadata.
            ContainerType DataElements;

            /// @brief Create an empty, invalid spline
            CubicSpline() {}

            /// @brief Create a Spline with a time series spread evenly between 0 and 1.
            /// @param y The data series.
            CubicSpline(const std::vector<DataSeriesType>& y)
            {
                std::vector<TimeSeriesType> x;


                CalculateElements(x,y);
            }

            /// @brief A spline with custom time and data series values
            CubicSpline(const std::vector<TimeSeriesType>& x, const std::vector<DataSeriesType>& y)
                { CalculateElements(x,y); }

            virtual ~CubicSpline() {}

            DataSeriesType operator[](const TimeSeriesType& x) const
                { return interpolate(x); }

            DataSeriesType interpolate(const TimeSeriesType&x) const
            {
                if (DataElements.size() == 0)
                    { return DataSeriesType(); }
                typename std::vector<element_type>::const_iterator it;
                it = std::lower_bound(DataElements.begin(), DataElements.end(), element_type(x));
                if (it != DataElements.begin())
                    { it--; }
                return it->eval(x);
            }

            std::vector<DataSeriesType> operator[](const std::vector<TimeSeriesType>& xx) const
                { return interpolate(xx); }

            iterator begin()
                { return DataElements.begin(); }
            iterator end()
                { return DataElements.end(); }

            const_iterator begin() const
                { return DataElements.begin(); }
            const_iterator end() const
                { return DataElements.end(); }

            /// @brief Add another entry to the spline
            /// @details Adjust the time series to evenly distribute it between 0 and 1 for
            /// each entry, including the new one DataToAdd
            void push_back(const DataSeriesType& DataToAdd)
            {
                // This needs to be implemented still.
                std::vector<TimeSeriesType> x;
                std::vector<DataSeriesType> y;


                CalculateElements(x,y);
            }

            /* Evaluate at multiple locations, assuming xx is sorted ascending */
            std::vector<DataSeriesType> interpolate(const std::vector<TimeSeriesType>& xx) const
            {
                if (DataElements.size() == 0)
                    { return std::vector<DataSeriesType>(xx.size()); }

                typename std::vector<TimeSeriesType>::const_iterator it;
                typename std::vector<element_type>::const_iterator it2;
                it2 = DataElements.begin();
                std::vector<DataSeriesType> ys;
                for (it = xx.begin(); it != xx.end(); it++)
                {
                    it2 = std::lower_bound(it2, DataElements.end(), element_type(*it));
                    if (it2 != DataElements.begin())
                        { it2--; }

                    ys.push_back(it2->eval(*it));
                }
                return ys;
            }

        protected:

            void CalculateElements(const std::vector<TimeSeriesType>& x, const std::vector<DataSeriesType>& y)
            {
                if (x.size() != y.size())
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Data series and time series must have the same count of elements."); }

                if (x.size() < 3)
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Must have at least three points for interpolation."); }

                DataElements.clear();

                typedef typename std::vector<TimeSeriesType>::difference_type size_type;

                size_type n = y.size() - 1;

                std::vector<DataSeriesType> b(n), d(n), a(n), c(n+1), l(n+1), u(n+1), z(n+1);
                std::vector<TimeSeriesType> h(n+1);

                l[0] = DataSeriesType(1);
                u[0] = DataSeriesType(0);
                z[0] = DataSeriesType(0);
                h[0] = x[1] - x[0];

                for (size_type i = 1; i < n; i++)
                {
                    h[i] = x[i+1] - x[i];
                    l[i] = DataSeriesType(2 * (x[i+1] - x[i-1])) - DataSeriesType(h[i-1]) * u[i-1];
                    u[i] = DataSeriesType(h[i]) / l[i];
                    a[i] = (DataSeriesType(3) / DataSeriesType(h[i])) * (y[i+1] - y[i]) - (DataSeriesType(3) / DataSeriesType(h[i-1])) * (y[i] - y[i-1]);
                    z[i] = (a[i] - DataSeriesType(h[i-1]) * z[i-1]) / l[i];
                }

                l[n] = DataSeriesType(1);
                z[n] = c[n] = DataSeriesType(0);

                for (size_type j = n-1; j >= 0; j--)
                {
                    c[j] = z[j] - u[j] * c[j+1];
                    b[j] = (y[j+1] - y[j]) / DataSeriesType(h[j]) - (DataSeriesType(h[j]) * (c[j+1] + DataSeriesType(2) * c[j])) / DataSeriesType(3);
                    d[j] = (c[j+1] - c[j]) / DataSeriesType(3 * h[j]);
                }

                for (size_type i = 0; i < n; i++)
                {
                    DataElements.push_back(element_type(x[i], y[i], b[i], c[i], d[i]));
                }
            }
    };

} // /namespace Mezzanine

#endif // Include guard
