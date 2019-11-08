<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.3" name="FlyingEnemy" tilewidth="32" tileheight="32" tilecount="2" columns="2">
 <image source="textures/bat.png" width="64" height="32"/>
 <tile id="0" type="idle">
  <properties>
   <property name="speed" type="float" value="2"/>
  </properties>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
  </animation>
 </tile>
</tileset>
