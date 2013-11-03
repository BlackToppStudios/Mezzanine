//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _track_cpp
#define _track_cpp

#include "track.h"

#include <algorithm>

namespace Mezzanine
{
    TrackOld::TrackOld()
        : Closed(false)
    {
    }

    TrackOld::~TrackOld()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void TrackOld::AddPoint(const Vector3& Point)
    {
        this->Points.push_back(Point);
    }

    void TrackOld::AddPoint(const Real X, const Real Y, const Real Z)
    {
        this->AddPoint(Vector3(X,Y,Z));
    }

    void TrackOld::UpdatePoint(const UInt32 Index, const Vector3& Point)
    {
        this->Points.at(Index) = Point;
    }

    void TrackOld::UpdatePoint(const UInt32 Index, const Real X, const Real Y, const Real Z)
    {
        this->UpdatePoint(Index,Vector3(X,Y,Z));
    }

    const Vector3& TrackOld::GetPoint(const UInt32 Index) const
    {
        return this->Points.at(Index);
    }

    UInt32 TrackOld::GetNumPoints() const
    {
        return this->Points.size();
    }

    void TrackOld::ClearAllPoints()
    {
        this->Points.clear();
    }

    void TrackOld::Close(bool Enable)
    {
        this->Closed = Enable;
    }

    bool TrackOld::IsClosed() const
    {
        return this->Closed;
    }

    void TrackOld::Reverse()
    {
        std::reverse(Points.begin(),Points.end());
    }

    TrackOld TrackOld::ExtractSubPath(const UInt32 First, const UInt32 Last)
    {
        TrackOld NewTrack;
        NewTrack.Points.insert( NewTrack.Points.end(), this->Points.begin() + First, this->Points.begin() + Last + 1 );
        return NewTrack;
    }

    TrackOld::PointContainer& TrackOld::GetPoints()
    {
        return this->Points;
    }

    const TrackOld::PointContainer& TrackOld::GetPoints() const
    {
        return this->Points;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Transform related methods

    void TrackOld::Translate(const Vector3& Trans)
    {
        for( PointIterator PointIt = this->Points.begin() ; PointIt != this->Points.end() ; ++PointIt )
        {
            (*PointIt) = (*PointIt) + Trans;
        }
    }

    void TrackOld::Translate(const Real X, const Real Y, const Real Z)
    {
        this->Translate(Vector3(X,Y,Z));
    }

    void TrackOld::Scale(const Vector3& Sc)
    {
        for( PointIterator PointIt = this->Points.begin() ; PointIt != this->Points.end() ; ++PointIt )
        {
            (*PointIt) *= Sc.X;
            (*PointIt) *= Sc.Y;
            (*PointIt) *= Sc.Z;
        }
    }

    void TrackOld::Scale(const Real X, const Real Y, const Real Z)
    {
        for( PointIterator PointIt = this->Points.begin() ; PointIt != this->Points.end() ; ++PointIt )
        {
            (*PointIt) *= X;
            (*PointIt) *= Y;
            (*PointIt) *= Z;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Interpolation

    Vector3 TrackOld::GetLocationOnTrack(const Real TrackPos)
    {
        return Vector3(0,0,0);
    }

    Vector3 TrackOld::GetLocationOnTrack(const UInt32 Index, const Real TrackPos)
    {
        return Vector3(0,0,0);
    }
}//Mezzanine

#endif

