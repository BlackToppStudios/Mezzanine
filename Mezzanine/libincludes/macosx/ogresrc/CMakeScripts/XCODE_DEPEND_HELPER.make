# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to, avoiding a bug in XCode 1.5
all.Debug: \
	$(SRCROOT)/lib/Debug/Sample_BSP.dylib\
	$(SRCROOT)/lib/Debug/Sample_CelShading.dylib\
	$(SRCROOT)/lib/Debug/Sample_BezierPatch.dylib\
	$(SRCROOT)/lib/Debug/Sample_CameraTrack.dylib\
	$(SRCROOT)/lib/Debug/Sample_Character.dylib\
	$(SRCROOT)/lib/Debug/Sample_Compositor.dylib\
	$(SRCROOT)/lib/Debug/Sample_CubeMapping.dylib\
	$(SRCROOT)/lib/Debug/Sample_Dot3Bump.dylib\
	$(SRCROOT)/lib/Debug/Sample_DynTex.dylib\
	$(SRCROOT)/lib/Debug/Sample_FacialAnimation.dylib\
	$(SRCROOT)/lib/Debug/Sample_Fresnel.dylib\
	$(SRCROOT)/lib/Debug/Sample_Grass.dylib\
	$(SRCROOT)/lib/Debug/Sample_Instancing.dylib\
	$(SRCROOT)/lib/Debug/Sample_Lighting.dylib\
	$(SRCROOT)/lib/Debug/Sample_Ocean.dylib\
	$(SRCROOT)/lib/Debug/Sample_ParticleFX.dylib\
	$(SRCROOT)/lib/Debug/Sample_ShaderSystem.dylib\
	$(SRCROOT)/lib/Debug/Sample_Shadows.dylib\
	$(SRCROOT)/lib/Debug/Sample_SkeletalAnimation.dylib\
	$(SRCROOT)/lib/Debug/Sample_SkyBox.dylib\
	$(SRCROOT)/lib/Debug/Sample_SkyDome.dylib\
	$(SRCROOT)/lib/Debug/Sample_SkyPlane.dylib\
	$(SRCROOT)/lib/Debug/Sample_Smoke.dylib\
	$(SRCROOT)/lib/Debug/Sample_SphereMapping.dylib\
	$(SRCROOT)/lib/Debug/Sample_Terrain.dylib\
	$(SRCROOT)/lib/Debug/Sample_TextureFX.dylib\
	$(SRCROOT)/lib/Debug/Sample_Transparency.dylib\
	$(SRCROOT)/lib/Debug/Sample_VolumeTex.dylib\
	$(SRCROOT)/lib/Debug/Sample_Water.dylib\
	$(SRCROOT)/lib/Debug/Sample_DeferredShading.dylib\
	$(SRCROOT)/lib/Debug/Sample_Isosurf.dylib\
	$(SRCROOT)/lib/Debug/Sample_ParticleGS.dylib\
	$(SRCROOT)/bin/Debug/SampleBrowser.app/Contents/MacOS/SampleBrowser

all.Release: \
	$(SRCROOT)/lib/Release/Sample_BSP.dylib\
	$(SRCROOT)/lib/Release/Sample_CelShading.dylib\
	$(SRCROOT)/lib/Release/Sample_BezierPatch.dylib\
	$(SRCROOT)/lib/Release/Sample_CameraTrack.dylib\
	$(SRCROOT)/lib/Release/Sample_Character.dylib\
	$(SRCROOT)/lib/Release/Sample_Compositor.dylib\
	$(SRCROOT)/lib/Release/Sample_CubeMapping.dylib\
	$(SRCROOT)/lib/Release/Sample_Dot3Bump.dylib\
	$(SRCROOT)/lib/Release/Sample_DynTex.dylib\
	$(SRCROOT)/lib/Release/Sample_FacialAnimation.dylib\
	$(SRCROOT)/lib/Release/Sample_Fresnel.dylib\
	$(SRCROOT)/lib/Release/Sample_Grass.dylib\
	$(SRCROOT)/lib/Release/Sample_Instancing.dylib\
	$(SRCROOT)/lib/Release/Sample_Lighting.dylib\
	$(SRCROOT)/lib/Release/Sample_Ocean.dylib\
	$(SRCROOT)/lib/Release/Sample_ParticleFX.dylib\
	$(SRCROOT)/lib/Release/Sample_ShaderSystem.dylib\
	$(SRCROOT)/lib/Release/Sample_Shadows.dylib\
	$(SRCROOT)/lib/Release/Sample_SkeletalAnimation.dylib\
	$(SRCROOT)/lib/Release/Sample_SkyBox.dylib\
	$(SRCROOT)/lib/Release/Sample_SkyDome.dylib\
	$(SRCROOT)/lib/Release/Sample_SkyPlane.dylib\
	$(SRCROOT)/lib/Release/Sample_Smoke.dylib\
	$(SRCROOT)/lib/Release/Sample_SphereMapping.dylib\
	$(SRCROOT)/lib/Release/Sample_Terrain.dylib\
	$(SRCROOT)/lib/Release/Sample_TextureFX.dylib\
	$(SRCROOT)/lib/Release/Sample_Transparency.dylib\
	$(SRCROOT)/lib/Release/Sample_VolumeTex.dylib\
	$(SRCROOT)/lib/Release/Sample_Water.dylib\
	$(SRCROOT)/lib/Release/Sample_DeferredShading.dylib\
	$(SRCROOT)/lib/Release/Sample_Isosurf.dylib\
	$(SRCROOT)/lib/Release/Sample_ParticleGS.dylib\
	$(SRCROOT)/bin/Release/SampleBrowser.app/Contents/MacOS/SampleBrowser

all.MinSizeRel: \
	$(SRCROOT)/lib/MinSizeRel/Sample_BSP.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_CelShading.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_BezierPatch.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_CameraTrack.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Character.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Compositor.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_CubeMapping.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Dot3Bump.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_DynTex.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_FacialAnimation.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Fresnel.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Grass.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Instancing.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Lighting.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Ocean.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_ParticleFX.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_ShaderSystem.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Shadows.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_SkeletalAnimation.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_SkyBox.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_SkyDome.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_SkyPlane.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Smoke.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_SphereMapping.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Terrain.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_TextureFX.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Transparency.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_VolumeTex.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Water.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_DeferredShading.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_Isosurf.dylib\
	$(SRCROOT)/lib/MinSizeRel/Sample_ParticleGS.dylib\
	$(SRCROOT)/bin/MinSizeRel/SampleBrowser.app/Contents/MacOS/SampleBrowser

all.RelWithDebInfo: \
	$(SRCROOT)/lib/RelWithDebInfo/Sample_BSP.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_CelShading.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_BezierPatch.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_CameraTrack.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Character.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Compositor.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_CubeMapping.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Dot3Bump.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_DynTex.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_FacialAnimation.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Fresnel.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Grass.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Instancing.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Lighting.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Ocean.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_ParticleFX.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_ShaderSystem.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Shadows.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_SkeletalAnimation.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_SkyBox.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_SkyDome.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_SkyPlane.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Smoke.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_SphereMapping.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Terrain.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_TextureFX.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Transparency.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_VolumeTex.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Water.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_DeferredShading.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_Isosurf.dylib\
	$(SRCROOT)/lib/RelWithDebInfo/Sample_ParticleGS.dylib\
	$(SRCROOT)/bin/RelWithDebInfo/SampleBrowser.app/Contents/MacOS/SampleBrowser

# For each target create a dummy rule so the target does not have to exist
$(SRCROOT)/lib/libOgreRTShaderSystem.dylib:
$(SRCROOT)/lib/release/libOIS.a:
$(SRCROOT)/lib/libOgreTerrain.dylib:


# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
$(SRCROOT)/lib/Debug/Sample_BSP.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_BSP.dylib


$(SRCROOT)/lib/Debug/Sample_CelShading.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_CelShading.dylib


$(SRCROOT)/lib/Debug/Sample_BezierPatch.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_BezierPatch.dylib


$(SRCROOT)/lib/Debug/Sample_CameraTrack.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_CameraTrack.dylib


$(SRCROOT)/lib/Debug/Sample_Character.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Character.dylib


$(SRCROOT)/lib/Debug/Sample_Compositor.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Compositor.dylib


$(SRCROOT)/lib/Debug/Sample_CubeMapping.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_CubeMapping.dylib


$(SRCROOT)/lib/Debug/Sample_Dot3Bump.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Dot3Bump.dylib


$(SRCROOT)/lib/Debug/Sample_DynTex.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_DynTex.dylib


$(SRCROOT)/lib/Debug/Sample_FacialAnimation.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_FacialAnimation.dylib


$(SRCROOT)/lib/Debug/Sample_Fresnel.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Fresnel.dylib


$(SRCROOT)/lib/Debug/Sample_Grass.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Grass.dylib


$(SRCROOT)/lib/Debug/Sample_Instancing.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Instancing.dylib


$(SRCROOT)/lib/Debug/Sample_Lighting.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Lighting.dylib


$(SRCROOT)/lib/Debug/Sample_Ocean.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Ocean.dylib


$(SRCROOT)/lib/Debug/Sample_ParticleFX.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_ParticleFX.dylib


$(SRCROOT)/lib/Debug/Sample_ShaderSystem.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_ShaderSystem.dylib


$(SRCROOT)/lib/Debug/Sample_Shadows.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Shadows.dylib


$(SRCROOT)/lib/Debug/Sample_SkeletalAnimation.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_SkeletalAnimation.dylib


$(SRCROOT)/lib/Debug/Sample_SkyBox.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_SkyBox.dylib


$(SRCROOT)/lib/Debug/Sample_SkyDome.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_SkyDome.dylib


$(SRCROOT)/lib/Debug/Sample_SkyPlane.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_SkyPlane.dylib


$(SRCROOT)/lib/Debug/Sample_Smoke.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Smoke.dylib


$(SRCROOT)/lib/Debug/Sample_SphereMapping.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_SphereMapping.dylib


$(SRCROOT)/lib/Debug/Sample_Terrain.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a\
	$(SRCROOT)/lib/libOgreTerrain.dylib\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a\
	$(SRCROOT)/lib/libOgreTerrain.dylib
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Terrain.dylib


$(SRCROOT)/lib/Debug/Sample_TextureFX.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_TextureFX.dylib


$(SRCROOT)/lib/Debug/Sample_Transparency.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Transparency.dylib


$(SRCROOT)/lib/Debug/Sample_VolumeTex.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_VolumeTex.dylib


$(SRCROOT)/lib/Debug/Sample_Water.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Water.dylib


$(SRCROOT)/lib/Debug/Sample_DeferredShading.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_DeferredShading.dylib


$(SRCROOT)/lib/Debug/Sample_Isosurf.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_Isosurf.dylib


$(SRCROOT)/lib/Debug/Sample_ParticleGS.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Debug/Sample_ParticleGS.dylib


$(SRCROOT)/bin/Debug/SampleBrowser.app/Contents/MacOS/SampleBrowser:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/bin/Debug/SampleBrowser.app/Contents/MacOS/SampleBrowser


$(SRCROOT)/lib/Release/Sample_BSP.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_BSP.dylib


$(SRCROOT)/lib/Release/Sample_CelShading.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_CelShading.dylib


$(SRCROOT)/lib/Release/Sample_BezierPatch.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_BezierPatch.dylib


$(SRCROOT)/lib/Release/Sample_CameraTrack.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_CameraTrack.dylib


$(SRCROOT)/lib/Release/Sample_Character.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Character.dylib


$(SRCROOT)/lib/Release/Sample_Compositor.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Compositor.dylib


$(SRCROOT)/lib/Release/Sample_CubeMapping.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_CubeMapping.dylib


$(SRCROOT)/lib/Release/Sample_Dot3Bump.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Dot3Bump.dylib


$(SRCROOT)/lib/Release/Sample_DynTex.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_DynTex.dylib


$(SRCROOT)/lib/Release/Sample_FacialAnimation.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_FacialAnimation.dylib


$(SRCROOT)/lib/Release/Sample_Fresnel.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Fresnel.dylib


$(SRCROOT)/lib/Release/Sample_Grass.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Grass.dylib


$(SRCROOT)/lib/Release/Sample_Instancing.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Instancing.dylib


$(SRCROOT)/lib/Release/Sample_Lighting.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Lighting.dylib


$(SRCROOT)/lib/Release/Sample_Ocean.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Ocean.dylib


$(SRCROOT)/lib/Release/Sample_ParticleFX.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_ParticleFX.dylib


$(SRCROOT)/lib/Release/Sample_ShaderSystem.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_ShaderSystem.dylib


$(SRCROOT)/lib/Release/Sample_Shadows.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Shadows.dylib


$(SRCROOT)/lib/Release/Sample_SkeletalAnimation.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_SkeletalAnimation.dylib


$(SRCROOT)/lib/Release/Sample_SkyBox.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_SkyBox.dylib


$(SRCROOT)/lib/Release/Sample_SkyDome.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_SkyDome.dylib


$(SRCROOT)/lib/Release/Sample_SkyPlane.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_SkyPlane.dylib


$(SRCROOT)/lib/Release/Sample_Smoke.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Smoke.dylib


$(SRCROOT)/lib/Release/Sample_SphereMapping.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_SphereMapping.dylib


$(SRCROOT)/lib/Release/Sample_Terrain.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a\
	$(SRCROOT)/lib/libOgreTerrain.dylib\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a\
	$(SRCROOT)/lib/libOgreTerrain.dylib
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Terrain.dylib


$(SRCROOT)/lib/Release/Sample_TextureFX.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_TextureFX.dylib


$(SRCROOT)/lib/Release/Sample_Transparency.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Transparency.dylib


$(SRCROOT)/lib/Release/Sample_VolumeTex.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_VolumeTex.dylib


$(SRCROOT)/lib/Release/Sample_Water.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Water.dylib


$(SRCROOT)/lib/Release/Sample_DeferredShading.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_DeferredShading.dylib


$(SRCROOT)/lib/Release/Sample_Isosurf.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_Isosurf.dylib


$(SRCROOT)/lib/Release/Sample_ParticleGS.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/Release/Sample_ParticleGS.dylib


$(SRCROOT)/bin/Release/SampleBrowser.app/Contents/MacOS/SampleBrowser:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/bin/Release/SampleBrowser.app/Contents/MacOS/SampleBrowser


$(SRCROOT)/lib/MinSizeRel/Sample_BSP.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_BSP.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_CelShading.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_CelShading.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_BezierPatch.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_BezierPatch.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_CameraTrack.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_CameraTrack.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Character.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Character.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Compositor.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Compositor.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_CubeMapping.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_CubeMapping.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Dot3Bump.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Dot3Bump.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_DynTex.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_DynTex.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_FacialAnimation.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_FacialAnimation.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Fresnel.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Fresnel.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Grass.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Grass.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Instancing.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Instancing.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Lighting.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Lighting.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Ocean.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Ocean.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_ParticleFX.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_ParticleFX.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_ShaderSystem.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_ShaderSystem.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Shadows.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Shadows.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_SkeletalAnimation.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_SkeletalAnimation.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_SkyBox.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_SkyBox.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_SkyDome.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_SkyDome.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_SkyPlane.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_SkyPlane.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Smoke.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Smoke.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_SphereMapping.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_SphereMapping.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Terrain.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a\
	$(SRCROOT)/lib/libOgreTerrain.dylib\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a\
	$(SRCROOT)/lib/libOgreTerrain.dylib
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Terrain.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_TextureFX.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_TextureFX.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Transparency.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Transparency.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_VolumeTex.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_VolumeTex.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Water.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Water.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_DeferredShading.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_DeferredShading.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_Isosurf.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_Isosurf.dylib


$(SRCROOT)/lib/MinSizeRel/Sample_ParticleGS.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/MinSizeRel/Sample_ParticleGS.dylib


$(SRCROOT)/bin/MinSizeRel/SampleBrowser.app/Contents/MacOS/SampleBrowser:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/bin/MinSizeRel/SampleBrowser.app/Contents/MacOS/SampleBrowser


$(SRCROOT)/lib/RelWithDebInfo/Sample_BSP.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_BSP.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_CelShading.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_CelShading.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_BezierPatch.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_BezierPatch.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_CameraTrack.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_CameraTrack.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Character.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Character.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Compositor.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Compositor.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_CubeMapping.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_CubeMapping.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Dot3Bump.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Dot3Bump.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_DynTex.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_DynTex.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_FacialAnimation.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_FacialAnimation.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Fresnel.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Fresnel.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Grass.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Grass.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Instancing.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Instancing.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Lighting.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Lighting.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Ocean.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Ocean.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_ParticleFX.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_ParticleFX.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_ShaderSystem.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_ShaderSystem.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Shadows.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Shadows.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_SkeletalAnimation.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_SkeletalAnimation.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_SkyBox.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_SkyBox.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_SkyDome.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_SkyDome.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_SkyPlane.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_SkyPlane.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Smoke.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Smoke.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_SphereMapping.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_SphereMapping.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Terrain.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a\
	$(SRCROOT)/lib/libOgreTerrain.dylib\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a\
	$(SRCROOT)/lib/libOgreTerrain.dylib
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Terrain.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_TextureFX.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_TextureFX.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Transparency.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Transparency.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_VolumeTex.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_VolumeTex.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Water.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Water.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_DeferredShading.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_DeferredShading.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_Isosurf.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_Isosurf.dylib


$(SRCROOT)/lib/RelWithDebInfo/Sample_ParticleGS.dylib:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/lib/RelWithDebInfo/Sample_ParticleGS.dylib


$(SRCROOT)/bin/RelWithDebInfo/SampleBrowser.app/Contents/MacOS/SampleBrowser:\
	$(SRCROOT)/lib/libOgreRTShaderSystem.dylib\
	$(SRCROOT)/lib/release/libOIS.a
	/bin/rm -f $(SRCROOT)/bin/RelWithDebInfo/SampleBrowser.app/Contents/MacOS/SampleBrowser


