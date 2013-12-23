// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
            bool operator<(const CubicSplineElement& e) const
                { return x < e.x; }

            /// @brief Sort Meta-data elements by time series locations
            /// @param e The CubicSplineEleme to compare this one too.
            bool operator<(const X& xx) const
                { return x < xx; }
    };

    /// @brief A class for interpolating data with arbitrary
    /// @details Templated on type of X, Y. X and Y must have operator +, -, *, /. Y must have defined
    /// a constructor that takes a scalar.
    template <typename X, typename Y>
    class MEZZ_LIB CubicSpline
    {
        public:
            typedef CubicSplineElement<X,Y> element_type;
            std::vector<element_type> mElements;

            /** An empty, invalid spline */
            CubicSpline() {}

            /** A spline with x and y values */
            CubicSpline(const std::vector<X>& x, const std::vector<Y>& y)
                { CalculateElements(x,y); }

            virtual ~CubicSpline() {}

            Y operator[](const X& x) const {
                return interpolate(x);
            }

            Y interpolate(const X&x) const {
                if (mElements.size() == 0) return Y();

                typename std::vector<element_type>::const_iterator it;
                it = std::lower_bound(mElements.begin(), mElements.end(), element_type(x));
                if (it != mElements.begin()) {
                    it--;
                }

                return it->eval(x);
            }

            std::vector<Y> operator[](const std::vector<X>& xx) const {
                return interpolate(xx);
            }

            /* Evaluate at multiple locations, assuming xx is sorted ascending */
            std::vector<Y> interpolate(const std::vector<X>& xx) const {
                if (mElements.size() == 0) return std::vector<Y>(xx.size());

                typename std::vector<X>::const_iterator it;
                typename std::vector<element_type>::const_iterator it2;
                it2 = mElements.begin();
                std::vector<Y> ys;
                for (it = xx.begin(); it != xx.end(); it++) {
                    it2 = std::lower_bound(it2, mElements.end(), element_type(*it));
                    if (it2 != mElements.begin()) {
                        it2--;
                    }

                    ys.push_back(it2->eval(*it));
                }

                return ys;
            }

        protected:

            void CalculateElements(const std::vector<X>& x, const std::vector<Y>& y)
            {
                if (x.size() != y.size())
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"data series and time series must have the same count of elemens."); }

                if (x.size() < 3)
                    { MEZZ_EXCEPTION(Exception::PARAMETERS_RANGE_EXCEPTION,"Must have at least three points for interpolation."); }

                typedef typename std::vector<X>::difference_type size_type;

                size_type n = y.size() - 1;

                std::vector<Y> b(n), d(n), a(n), c(n+1), l(n+1), u(n+1), z(n+1);
                std::vector<X> h(n+1);

                l[0] = Y(1);
                u[0] = Y(0);
                z[0] = Y(0);
                h[0] = x[1] - x[0];

                for (size_type i = 1; i < n; i++)
                {
                    h[i] = x[i+1] - x[i];
                    l[i] = Y(2 * (x[i+1] - x[i-1])) - Y(h[i-1]) * u[i-1];
                    u[i] = Y(h[i]) / l[i];
                    a[i] = (Y(3) / Y(h[i])) * (y[i+1] - y[i]) - (Y(3) / Y(h[i-1])) * (y[i] - y[i-1]);
                    z[i] = (a[i] - Y(h[i-1]) * z[i-1]) / l[i];
                }

                l[n] = Y(1);
                z[n] = c[n] = Y(0);

                for (size_type j = n-1; j >= 0; j--)
                {
                    c[j] = z[j] - u[j] * c[j+1];
                    b[j] = (y[j+1] - y[j]) / Y(h[j]) - (Y(h[j]) * (c[j+1] + Y(2) * c[j])) / Y(3);
                    d[j] = (c[j+1] - c[j]) / Y(3 * h[j]);
                }

                for (size_type i = 0; i < n; i++)
                {
                    mElements.push_back(element_type(x[i], y[i], b[i], c[i], d[i]));
                }
            }
    };

} // /namespace Mezzanine

#endif // Include guard
