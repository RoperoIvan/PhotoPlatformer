<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.0" name="Player" tilewidth="34" tileheight="64" tilecount="25" columns="25">
 <image source="textures/Player2.png" width="850" height="64"/>
 <tile id="0" type="idle">
  <properties>
   <property name="speed" type="float" value="0.7"/>
  </properties>
  <animation>
   <frame tileid="0" duration="2000"/>
   <frame tileid="1" duration="2000"/>
   <frame tileid="2" duration="2000"/>
  </animation>
 </tile>
 <tile id="3" type="walking">
  <properties>
   <property name="speed" type="float" value="1.5"/>
  </properties>
  <animation>
   <frame tileid="3" duration="200"/>
   <frame tileid="4" duration="200"/>
   <frame tileid="5" duration="200"/>
   <frame tileid="6" duration="200"/>
  </animation>
 </tile>
 <tile id="7" type="jump">
  <properties>
   <property name="speed" type="float" value="0.1"/>
  </properties>
  <animation>
   <frame tileid="7" duration="100"/>
   <frame tileid="8" duration="100"/>
   <frame tileid="9" duration="100"/>
  </animation>
 </tile>
 <tile id="10">
  <animation>
   <frame tileid="10" duration="200"/>
   <frame tileid="11" duration="200"/>
  </animation>
 </tile>
 <tile id="12" type="death">
  <animation>
   <frame tileid="12" duration="100"/>
   <frame tileid="13" duration="100"/>
   <frame tileid="14" duration="100"/>
   <frame tileid="15" duration="100"/>
   <frame tileid="16" duration="100"/>
   <frame tileid="17" duration="100"/>
   <frame tileid="18" duration="100"/>
   <frame tileid="19" duration="100"/>
   <frame tileid="20" duration="100"/>
   <frame tileid="21" duration="100"/>
   <frame tileid="22" duration="100"/>
   <frame tileid="23" duration="100"/>
   <frame tileid="24" duration="100"/>
  </animation>
 </tile>
</tileset>
