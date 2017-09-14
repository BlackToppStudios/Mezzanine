// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
/*
 *
 * Software, Files, Libraries and all other items referenced in this clause refers only
 * to the contents of this file and associated documentation.
 *
 * Pugixml parser - version 1.0
 * --------------------------------------------------------
 * Copyright © 2006-2012, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at http://pugixml.org/
 *
 * This library is distributed under the MIT License. See notice at the end
 * of this file.
 *
 * This work is based on the pugxml parser, which is:
 * Copyright © 2003, by Kristen Wegner (kristen@tima.net)
 */
#ifndef _xmltreewalker_h
#define _xmltreewalker_h

/// @file
/// @brief This defines the TreeWalker Interface

#include "datatypes.h"
#include "XML/node.h"



namespace Mezzanine
{
    namespace XML
    {
        //////////////////////////////////////////////////////////////////////////////
        /// @brief Used to call a function OnEachNode member of the subtree of nodes descended from a specific node.
        /// @details If you want to do a deep tree traversal, you'll either have to do it via a recursive function or some
        /// equivalent method or use a TreeWalker. This provides a helper for depth-first traversal of a subtree. In order
        /// to use it, you have to implement XML::TreeWalker interface and call XML::Node::Traverse() function. \n\n
        ///  * First, TreeWalker::OnTraversalBegin() is called with traversal root as its argument.\n
        ///  * Then, TreeWalker::OnEachNode() function is called for all nodes in the traversal subtree in depth first order, excluding the traversal root. Each Node is passed as an argument.\n
        ///  * Finally, TreeWalker::OnTraversalEnd() function is called with traversal root as its argument.\n\n
        /// If TreeWalker::OnTraversalBegin(), TreeWalker::OnTraversalEnd() or any of the TreeWalker::OnEachNode() calls return false, the traversal
        /// is terminated and false is returned as the traversal result; otherwise, the traversal results in true. Note that
        /// you don't have to override begin or end functions; their default implementations return true.\n\n
        /// You can get the node's depth relative to the traversal root at any point by calling TreeWalker::Depth() function.
        class MEZZ_LIB TreeWalker
        {
                friend class Node;

            private:

                /// @internal
                /// @brief Stores How deep in the tree we are
                int TraversalDepth;

            protected:

                /// @brief How many descendants deep are we during traversal.
                /// @return This returns -1 if called from TreeWalker::OnTraversalBegin() or TreeWalker::OnTraversalEnd(), and returns 0-based depth if called from OnEachNode - depth is 0 for all children of the traversal root, 1 for all grandchildren, 2 for great-grandchildren and so on.
                int Depth() const;

            public:

                /// @brief Default constructor, initializes depth, and can do little else without a fully implemented treewalker.
                TreeWalker();

                /// @brief Virtual deconstructor. Tears down a TreeWalker
                virtual ~TreeWalker();

                /// @brief Called by the root Node of the xml subtree when traversal begins.
                /// @param node The first node the Tree to traverse
                /// @details By default this simply returns true, but is expected to be overridden with any desired behavior
                /// @return True by default. If it returns false, then traversal ends and the Node::Traverse() that was called is expected to return false.
                virtual bool OnTraversalBegin(Node& node);

                /// @brief A Pure Virtual function that is expected to be implemented to create the desired behavior.
                /// @param node The curren node being trraversed.
                /// @details This is called on every Node that is traversed except the root node of the traversed subtree. Can be used to perform sophisticated searches
                /// of a portion of the xml document, alter the document on a large scale, gather statistics, or just about any other behavior that requires touching
                /// many nodes.
                /// @return if true Traversal is expected to continue, if false, then traversal ends and the Node::Traverse() that was called is expected to return false.
                virtual bool OnEachNode(Node& node) = 0;

                /// @brief Called on the root Node of the xml subtree when traversal ends.
                /// @param node The last node the Tree to traverse
                /// @details By default this simply returns true, but is expected to be overridden with any desired behavior
                /// @return True by default. If it returns false, then traversal ends and the Node::Traverse() that was called is expected to return false.
                virtual bool OnTraversalEnd(Node& node);
        };
    }
}// /namespace Mezzanine



#endif // Include guard

/*
 *
 * Software, Files, Libraries and all other items referenced in this clause refers only
 * to the contents of this file and associated documentation.
 *
 * Copyright © 2006-2012 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
