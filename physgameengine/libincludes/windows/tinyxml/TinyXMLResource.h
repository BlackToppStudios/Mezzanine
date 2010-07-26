// TinyXMLResourceManager.cpp class for Ogre
// Author: xadhoom
// License: Do whatever you want with it.

#ifndef __TinyXMLResource_h__
#define __TinyXMLResource_h__

#include "Ogre.h"


// Forward declaration
class TinyXMLPtr;

class TinyXMLResource : public Ogre::Resource
{
public:

   /** Constructor - use resource manager's create method rather than this.
   */
   TinyXMLResource(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
      const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0);

   ~TinyXMLResource();

   /** Assignment operator to allow easy copying between TinyXMLResources.
   */
   TinyXMLResource& operator=( const TinyXMLResource& rhs );

   /** Creates a new copy of this TinyXMLResource with the same settings but a new name.
   @param newName The name for the cloned TinyXMLResource
   @param changeGroup If true, the resource group of the clone is changed
   @param newGroup Only required if changeGroup is true; the new group to assign
   */
   TinyXMLPtr clone(const Ogre::String& newName, bool changeGroup = false, 
      const Ogre::String& newGroup = Ogre::StringUtil::BLANK);

   /** Returns the actual TinyXML document.
   @remarks
   The return value is upcasted to an TiXmlNode to have a consistent
   interface without the file input/output functionality of TinyXML.
   */
   TiXmlNode* getXMLData() { return &mTinyXMLDoc; }

   /**  If, an error occurs during parsing, Error() will return true.
   */
   bool getError() const { return mTinyXMLDoc.Error(); }

   /**  Returns a textual (english) description of the error if one occured.
   */
   const char * getErrorDesc() const { return mTinyXMLDoc.ErrorDesc(); }

   /** Generally, you probably want the error string ( ErrorDesc() ). But if you
   prefer the ErrorId, this function will fetch it.
   */
   const int getErrorId() const { return mTinyXMLDoc.ErrorId(); }

protected:

   /** Overridden from Resource.
   */
   void prepareImpl();

   /** Overridden from Resource.
   */
   void unprepareImpl();

   /** Overridden from Resource.
   */
   void loadImpl();

   /** Unloads the TinyXMLResource, frees resources etc.
   @see
   Resource
   */
   void unloadImpl();

   size_t calculateSize() const { return 0; } // TODO 

   Ogre::DataStreamPtr  mFreshFromDisk;

   TiXmlDocument        mTinyXMLDoc;
};


/** Specialisation of Ogre::SharedPtr to allow Ogre::SharedPtr to be assigned to TinyXMLPtr 
@note Has to be a subclass since we need operator=.
We could templatise this instead of repeating per Resource subclass, 
except to do so requires a form VC6 does not support i.e.
ResourceSubclassPtr<T> : public Ogre::SharedPtr<T>
*/
class TinyXMLPtr : public Ogre::SharedPtr<TinyXMLResource> 
{
public:
   TinyXMLPtr() : Ogre::SharedPtr<TinyXMLResource>() {}
   explicit TinyXMLPtr(TinyXMLResource* rep) : Ogre::SharedPtr<TinyXMLResource>(rep) {}
   TinyXMLPtr(const TinyXMLPtr& r) : Ogre::SharedPtr<TinyXMLResource>(r) {} 
   TinyXMLPtr(const Ogre::ResourcePtr& r);
   /// Operator used to convert a ResourcePtr to a TinyXMLPtr
   TinyXMLPtr& operator=(const Ogre::ResourcePtr& r);
};

#endif  // __TinyXMLPtr_h__