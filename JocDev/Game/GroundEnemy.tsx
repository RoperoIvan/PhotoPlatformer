<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.0" name="GroundEnemy" tilewidth="64" tileheight="64" tilecount="16" columns="4">
 <image source="textures/Slime_Medium_White.png" width="256" height="256"/>
 <tile id="0" type="idle">
  <properties>
   <property name="speed" type="float" value="6"/>
  </properties>
  <animation>
   <frame tileid="0" duration="200"/>
   <frame tileid="1" duration="200"/>
   <frame tileid="2" duration="200"/>
   <frame tileid="3" duration="200"/>
  </animation>
 </tile>
 <tile id="4" type="walking">
  <properties>
   <property name="speed" type="float" value="6"/>
  </properties>
  <animation>
   <frame tileid="4" duration="200"/>
   <frame tileid="5" duration="200"/>
   <frame tileid="6" duration="200"/>
   <frame tileid="7" duration="200"/>
  </animation>
 </tile>
 <tile id="8" type="death">
  <animation>
   <frame tileid="9" duration="200"/>
  </animation>
 </tile>
</tileset>
