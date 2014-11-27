Content (models / etc) are stored in OgreTank/Content
Materials/1.material
Materials/baseMaterial.material
Materials/CoinMat.material
Materials/Examples.material
Materials/Material.001.material
Materials/Material.003.material
Materials/Material.004.material
Materials/Material.material
Materials/myShellMaterial.001.material
Materials/myWallMaterial.material
Materials/PongMaterial.material
Materials/SolidColor.material
Materials/1.PNG
Materials/2.PNG
Materials/4.PNG
Materials/coinGraphic2.PNG
Materials/frost.PNG
Materials/layout_gimp.PNG
Materials/pong.PNG
Materials/RustySteel.JPEG
Materials/Scratched_Steel_Texture_by_AaronDesign.JPEG

Models/base.mesh
Models/Body.mesh
Models/Gun.mesh
Models/myShell.mesh
Models/myWall.mesh
Models/Turret.mesh

Overlays/Example.overlay

Configuration files are stored in OgreTank/Content, and are copied into the 
ouput directory as a post-build step.


If you change directory structure, be sure to modify resources.cfg and resources_d.cfg
to point at the correct places.



Physics:
Build Bullet Physics engine
Create environment variable BULLET_HOME


OpenAL:
First of all, you need to install OpenAL, then create environment variable OPENAL_SDK.
Secondly, you need to install ogreoggsound, then create environment variable OGG_SOUND_HOME.
Moreover, you need to download alut.dll, OpenAL32.dll and OgreOggSound_d.dll, then put them into C:\Windows\System32.
Finally, you need to download oalinst.exe from this link http://support.focus-home.fr/oalinst.exe.


Control tank:

Move tank up: Up button
Move tank Down: Down button
Move tank right: Right button
Move tank left: Left button
Move Turret right: D 
Move Turret right: A
Fire: Space
