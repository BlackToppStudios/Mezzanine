#ifndef PHYSWORLDCALLBACKMANAGER_H
#define PHYSWORLDCALLBACKMANAGER_H
///////////////////////////////////////////////////////////////////////////////
//Framelisteners are classes which contain callback functions for when
//certain events happen with the Rendering engine
///////////////////////////////////////////////////////////////////////////////

//#include "physworld.h"
class PhysWorld;

///////////////////////////////////////////////////////////////////////////////
// Interface of PhysWorldFrameListener
class PhysWorldCallBackManager
{
	private:
		//pointers to internal structures
		PhysWorld* TheWorldIListenTo;

		//pointers to callback functions
 		bool (*PreInputCallback)();
 		bool (*PrePhysicsCallback)();
 		bool (*PreRenderCallback)();
 		bool (*PostRenderCallback)();

	public:
	//To build a frame listener we need a world to listen too
  		PhysWorldCallBackManager(PhysWorld* _Parent);
  		~PhysWorldCallBackManager();

  		//Called when a frame is about to begin rendering. so is the
  		//Function you passe in with the setFrameStartCallback
		bool PreInput();
		void ErasePreInput();
		void SetPreInput(bool (*Callback)());
		bool IsPreInputCallbackSet();

		bool PrePhysics();
		void ErasePrePhysics();
		void SetPrePhysics(bool (*Callback)());
		bool IsPrePhysicsCallbackSet();

		bool PreRender();
		void ErasePreRender();
		void SetPreRender(bool (*Callback)());
		bool IsPreRenderCallbackSet();

		bool PostRender();
		void ErasePostRender();
		void SetPostRender(bool (*Callback)());
		bool IsPostRenderCallbackSet();


		friend class PhysWorld;
};

#endif
