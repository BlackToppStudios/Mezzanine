// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
// Copyright (C) 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#ifndef noisemodulemodulebase_h
#define noisemodulemodulebase_h

#include <assert.h>

#include "datatypes.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Noise
    {
        namespace Module
        {
            /// @addtogroup libnoise
            /// @{

            /// @defgroup modules Noise Modules
            /// @addtogroup modules
            /// @{

            /// Abstract base class for noise modules.
            ///
            /// A <i>noise module</i> is an object that calculates and outputs a value
            /// given a three-dimensional input value.
            ///
            /// Each type of noise module uses a specific method to calculate an
            /// output value.  Some of these methods include:
            ///
            /// - Calculating a value using a coherent-noise function or some other
            ///   mathematical function.
            /// - Mathematically changing the output value from another noise module
            ///   in various ways.
            /// - Combining the output values from two noise modules in various ways.
            ///
            /// An application can use the output values from these noise modules in
            /// the following ways:
            ///
            /// - It can be used as an elevation value for a terrain height map
            /// - It can be used as a grayscale (or an RGB-channel) value for a
            ///   procedural texture
            /// - It can be used as a position value for controlling the movement of a
            ///   simulated lifeform.
            ///
            /// A noise module defines a near-infinite 3-dimensional texture.  Each
            /// position in this "texture" has a specific value.
            ///
            /// <b>Combining noise modules</b>
            ///
            /// Noise modules can be combined with other noise modules to generate
            /// complex output values.  A noise module that is used as a source of
            /// output values for another noise module is called a <i>source
            /// module</i>.  Each of these source modules may be connected to other
            /// source modules, and so on.
            ///
            /// There is no limit to the number of noise modules that can be connected
            /// together in this way.  However, each connected noise module increases
            /// the time required to calculate an output value.
            ///
            /// <b>Noise-module categories</b>
            ///
            /// The noise module classes that are included in libnoise can be roughly
            /// divided into five categories.
            ///
            /// <i>Generator Modules</i>
            ///
            /// A generator module outputs a value generated by a coherent-noise
            /// function or some other mathematical function.
            ///
            /// Examples of generator modules include:
            /// - Noise::Module::Const: Outputs a constant value.
            /// - Noise::Module::Perlin: Outputs a value generated by a Perlin-noise
            ///   function.
            /// - Noise::Module::Voronoi: Outputs a value generated by a Voronoi-cell
            ///   function.
            ///
            /// <i>Modifier Modules</i>
            ///
            /// A modifer module mathematically modifies the output value from a
            /// source module.
            ///
            /// Examples of modifier modules include:
            /// - Noise::Module::Curve: Maps the output value from the source module
            ///   onto an arbitrary function curve.
            /// - Noise::Module::Invert: Inverts the output value from the source
            ///   module.
            ///
            /// <i>Combiner Modules</i>
            ///
            /// A combiner module mathematically combines the output values from two
            /// or more source modules together.
            ///
            /// Examples of combiner modules include:
            /// - Noise::Module::Add: Adds the two output values from two source
            ///   modules.
            /// - Noise::Module::Max: Outputs the larger of the two output values from
            ///   two source modules.
            ///
            /// <i>Selector Modules</i>
            ///
            /// A selector module uses the output value from a <i>control module</i>
            /// to specify how to combine the output values from its source modules.
            ///
            /// Examples of selector modules include:
            /// - Noise::Module::Blend: Outputs a value that is linearly interpolated
            ///   between the output values from two source modules; the interpolation
            ///   weight is determined by the output value from the control module.
            /// - Noise::Module::Select: Outputs the value selected from one of two
            ///   source modules chosen by the output value from a control module.
            ///
            /// <i>Transformer Modules</i>
            ///
            /// A transformer module applies a transformation to the coordinates of
            /// the input value before retrieving the output value from the source
            /// module.  A transformer module does not modify the output value.
            ///
            /// Examples of transformer modules include:
            /// - RotatePoint: Rotates the coordinates of the input value around the
            ///   origin before retrieving the output value from the source module.
            /// - ScalePoint: Multiplies each coordinate of the input value by a
            ///   constant value before retrieving the output value from the source
            ///   module.
            ///
            /// <b>Connecting source modules to a noise module</b>
            ///
            /// An application connects a source module to a noise module by passing
            /// the source module to the SetSourceModule() method.
            ///
            /// The application must also pass an <i>index value</i> to
            /// SetSourceModule() as well.  An index value is a numeric identifier for
            /// that source module.  Index values are consecutively numbered starting
            /// at zero.
            ///
            /// To retrieve a reference to a source module, pass its index value to
            /// the GetSourceModule() method.
            ///
            /// Each noise module requires the attachment of a certain number of
            /// source modules before it can output a value.  For example, the
            /// Noise::Module::Add module requires two source modules, while the
            /// Noise::Module::Perlin module requires none.  Call the
            /// GetSourceModuleCount() method to retrieve the number of source modules
            /// required by that module.
            ///
            /// For non-selector modules, it usually does not matter which index value
            /// an application assigns to a particular source module, but for selector
            /// modules, the purpose of a source module is defined by its index value.
            /// For example, consider the Noise::Module::Select noise module, which
            /// requires three source modules.  The control module is the source
            /// module assigned an index value of 2.  The control module determines
            /// whether the noise module will output the value from the source module
            /// assigned an index value of 0 or the output value from the source
            /// module assigned an index value of 1.
            ///
            /// <b>Generating output values with a noise module</b>
            ///
            /// Once an application has connected all required source modules to a
            /// noise module, the application can now begin to generate output values
            /// with that noise module.
            ///
            /// To generate an output value, pass the ( @a X, @a Y, @a Z ) coordinates
            /// of an input value to the GetValue() method.
            ///
            /// <b>Using a noise module to generate terrain height maps or textures</b>
            ///
            /// One way to generate a terrain height map or a texture is to first
            /// allocate a 2-dimensional array of Realing-point values.  For each
            /// array element, pass the array subscripts as @a X and @a Y coordinates
            /// to the GetValue() method (leaving the @a Z coordinate set to zero) and
            /// place the resulting output value into the array element.
            ///
            /// <b>Creating your own noise modules</b>
            ///
            /// Create a class that publicly derives from Noise::Module::ModuleBase.
            ///
            /// In the constructor, call the base class' constructor while passing the
            /// return value from GetSourceModuleCount() to it.
            ///
            /// Override the GetSourceModuleCount() pure virtual method.  From this
            /// method, return the number of source modules required by your noise
            /// module.
            ///
            /// Override the GetValue() pure virtual method.  For generator modules,
            /// calculate and output a value given the coordinates of the input value.
            /// For other modules, retrieve the output values from each source module
            /// referenced in the protected @a m_pSourceModule array, mathematically
            /// combine those values, and return the combined value.
            ///
            /// When developing a noise module, you must ensure that your noise module
            /// does not modify any source module or control module connected to it; a
            /// noise module can only modify the output value from those source
            /// modules.  You must also ensure that if an application fails to connect
            /// all required source modules via the SetSourceModule() method and then
            /// attempts to call the GetValue() method, your module will raise an
            /// assertion.
            ///
            /// It shouldn't be too difficult to create your own noise module.  If you
            /// still have some problems, take a look at the source code for
            /// Noise::Module::Add, which is a very simple noise module.
            class ModuleBase
            {
            public:
                /// @brief Constructor.
                ModuleBase(const Integer SourceModuleCount);
                /// Destructor.
                virtual ~ModuleBase();

                /// Connects a source module to this noise module.
                ///
                /// @param Index An index value to assign to this source module.
                /// @param SourceModule The source module to attach.
                ///
                /// @pre The index value ranges from 0 to one less than the number of
                /// source modules required by this noise module.
                ///
                /// @exception Exception::PARAMETERS_EXCEPTION An invalid parameter was
                /// specified; see the preconditions for more information.
                ///
                /// A noise module mathematically combines the output values from the
                /// source modules to generate the value returned by GetValue().
                ///
                /// The index value to assign a source module is a unique identifier
                /// for that source module.  If an index value has already been
                /// assigned to a source module, this noise module replaces the old
                /// source module with the new source module.
                ///
                /// Before an application can call the GetValue() method, it must
                /// first connect all required source modules.  To determine the
                /// number of source modules required by this noise module, call the
                /// GetSourceModuleCount() method.
                ///
                /// This source module must exist throughout the lifetime of this
                /// noise module unless another source module replaces that source
                /// module.
                ///
                /// A noise module does not modify a source module; it only modifies
                /// its output values.
                virtual void SetSourceModule(const Integer Index, const ModuleBase& SourceModule)
                {
                    assert( this->m_pSourceModule != NULL );
                    if( Index >= this->GetSourceModuleCount() || Index < 0 ) {
                        MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"The requested module index is out of bounds.");
                    }
                    this->m_pSourceModule[Index] = &SourceModule;
                }
                /// Returns a reference to a source module connected to this noise
                /// module.
                ///
                /// @param Index The index value assigned to the source module.
                ///
                /// @return Returns a reference to the source module.
                ///
                /// @pre The index value ranges from 0 to one less than the number of
                /// source modules required by this noise module.
                /// @pre A source module with the specified index value has been added
                /// to this noise module via a call to SetSourceModule().
                ///
                /// @exception Exception::INVALID_STATE_EXCEPTION See the preconditions for more
                /// information.
                ///
                /// Each noise module requires the attachment of a certain number of
                /// source modules before an application can call the GetValue()
                /// method.
                virtual const ModuleBase& GetSourceModule(const Integer Index) const
                {
                    assert( this->m_pSourceModule != NULL );

                    // The following fix was provided by Will Hawkins:
                    //
                    //   m_pSourceModule[index] != NULL
                    //
                    // was incorrect; it should be:
                    //
                    //   m_pSourceModule[index] == NULL
                    if( Index >= this->GetSourceModuleCount() || Index < 0 || this->m_pSourceModule[Index] == NULL) {
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to retrieve module that is out of bounds, or none exist.");
                    }
                    return *(this->m_pSourceModule[Index]);
                }

                /// @brief Returns the number of source modules required by this noise module.
                /// @return Returns the number of source modules required by this noise module.
                virtual Integer GetSourceModuleCount() const = 0;

                /// Generates an output value given the coordinates of the specified
                /// input value.
                ///
                /// @param X The @a X coordinate of the input value.
                /// @param Y The @a Y coordinate of the input value.
                /// @param Z The @a Z coordinate of the input value.
                ///
                /// @return Returns the output value.
                ///
                /// @pre All source modules required by this noise module have been
                /// passed to the SetSourceModule() method.
                ///
                /// Before an application can call this method, it must first connect
                /// all required source modules via the SetSourceModule() method.  If
                /// these source modules are not connected to this noise module, this
                /// method raises a debug assertion.
                ///
                /// To determine the number of source modules required by this noise
                /// module, call the GetSourceModuleCount() method.
                virtual PreciseReal GetValue(const PreciseReal X, const PreciseReal Y, const PreciseReal Z) const = 0;
            protected:
                /// @internal
                /// @brief An array containing the pointers to each source module required by this noise module.
                const ModuleBase** m_pSourceModule;
            private:
                /// @brief Assignment operator.
                /// @remarks This assignment operator does nothing and cannot be overridden.
                /// This restriction is necessary because if this object was copied,
                /// all source modules assigned to this noise module would need to be
                /// copied as well.
                const ModuleBase& operator=(const ModuleBase& Other)
                {
                    return *this;
                }
            };//ModuleBase
            /// @}

            /// @}
        }//ModuleBase
    }//Noise
}//Mezzanine

#endif
