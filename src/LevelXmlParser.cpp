#include "LevelXmlParser.h"

#include <Ogre.h>
#include "StdAfx.h"
#include "tinyxml.h"

using namespace Ogre;

static Light* LoadLight( TiXmlElement *XMLLight, SceneManager *mSceneMgr )
{
   TiXmlElement *XMLDiffuse, *XMLSpecular, *XMLAttentuation, *XMLPosition;

   // Create a light (point | directional | spot | radPoint)
   Light* l = mSceneMgr->createLight( XMLLight->Attribute("name") );
   if( !XMLLight->Attribute("type") || String(XMLLight->Attribute("type")) == "point" )
      l->setType( Light::LT_POINT );
   else if( String(XMLLight->Attribute("type")) == "directional")
      l->setType( Light::LT_DIRECTIONAL );
   else if( String(XMLLight->Attribute("type")) == "spot")
      l->setType( Light::LT_SPOTLIGHT );
   else if( String(XMLLight->Attribute("type")) == "radPoint")
      l->setType( Light::LT_POINT );

   XMLDiffuse = XMLLight->FirstChildElement("colourDiffuse");
   if( XMLDiffuse ){
      ColourValue Diffuse;
      Diffuse.r = Ogre::StringConverter::parseReal( XMLDiffuse->Attribute("r") );
      Diffuse.g = Ogre::StringConverter::parseReal( XMLDiffuse->Attribute("g") );
      Diffuse.b = Ogre::StringConverter::parseReal( XMLDiffuse->Attribute("b") );
      Diffuse.a = 1;
      l->setDiffuseColour(Diffuse);
   }
   XMLSpecular = XMLLight->FirstChildElement("colourSpecular");
   if( XMLSpecular ){
      ColourValue Specular;
      Specular.r = Ogre::StringConverter::parseReal( XMLSpecular->Attribute("r") );
      Specular.g = Ogre::StringConverter::parseReal( XMLSpecular->Attribute("g") );
      Specular.b = Ogre::StringConverter::parseReal( XMLSpecular->Attribute("b") );
      Specular.a = 1;
      l->setSpecularColour(Specular);
   }

   XMLAttentuation = XMLLight->FirstChildElement("lightAttenuation");
   if( XMLAttentuation )
   {
      //get defaults incase not all values specified
      Real range, constant, linear, quadratic;
      range = l->getAttenuationRange();
      constant = l->getAttenuationConstant();
      linear = l->getAttenuationLinear();
      quadratic = l->getAttenuationQuadric();

      if( XMLAttentuation->Attribute("range") )
         range = StringConverter::parseReal( XMLAttentuation->Attribute("range") );
      if( XMLAttentuation->Attribute("constant") )
         constant = StringConverter::parseReal( XMLAttentuation->Attribute("constant") );
      if( XMLAttentuation->Attribute("linear") )
         linear = StringConverter::parseReal( XMLAttentuation->Attribute("linear") );
      if( XMLAttentuation->Attribute("quadratic") )
         quadratic = StringConverter::parseReal( XMLAttentuation->Attribute("quadratic") );

      l->setAttenuation( range, constant, linear, quadratic );
   }

   XMLPosition = XMLLight->FirstChildElement("position");
   if( XMLPosition ) {
      Vector3 p = Vector3(0,0,0);
      if( XMLPosition->Attribute("x") )
         p.x = StringConverter::parseReal( XMLPosition->Attribute("x") );
      if( XMLPosition->Attribute("y") )
         p.y = StringConverter::parseReal( XMLPosition->Attribute("y") );
      if( XMLPosition->Attribute("z") )
         p.z = StringConverter::parseReal( XMLPosition->Attribute("z") );

      l->setPosition( p );
   }

   //castShadows      (true | false) "true"
   l->setCastShadows( true );
   if( XMLLight->Attribute("visible") )
      if( String(XMLLight->Attribute("visible")) == "false" )
         l->setCastShadows( false );

   //visible         (true | false) "true"     
   l->setVisible( true );
   if( XMLLight->Attribute("visible") )
      if( String(XMLLight->Attribute("visible")) == "false" )
         l->setVisible( false );

   return l;
}

void LevelXmlParser::parseDotScene( const String &SceneName, const String& groupName, SceneManager *yourSceneMgr, OgreBulletDynamics::DynamicsWorld* mPhysicsScene, SceneNode *pAttachNode, String sPrependNode)
{
   // set up shared object values
   m_sPrependNode = sPrependNode;
   mSceneMgr = yourSceneMgr;

   TiXmlDocument   *XMLDoc;
   TiXmlElement   *XMLRoot, *XMLNodes;

   try
   {
      DataStreamPtr pStream = ResourceGroupManager::getSingleton().
         openResource( SceneName, groupName );

      String data = pStream->getAsString();
      // Open the .scene File
      XMLDoc = new TiXmlDocument();
      XMLDoc->Parse( data.c_str() );
      pStream->close();
      pStream.setNull();

      if( XMLDoc->Error() )
      {
         //We'll just log, and continue on gracefully
         LogManager::getSingleton().logMessage("[dotSceneLoader] The TiXmlDocument reported an error");
         delete XMLDoc;
         return;
      }
   }
   catch(...)
   {
      //We'll just log, and continue on gracefully
      LogManager::getSingleton().logMessage("[dotSceneLoader] Error creating TiXmlDocument");
      delete XMLDoc;
      return;
   }

   // Validate the File
   XMLRoot = XMLDoc->RootElement();
   if( String( XMLRoot->Value()) != "scene"  ) {
      LogManager::getSingleton().logMessage( "[dotSceneLoader]Error: Invalid .scene File. Missing <scene>" );
      delete XMLDoc;     
      return;
   }

   // figure out where to attach any nodes we create
   if(pAttachNode == NULL)
   {
      pAttachNode = mSceneMgr->getRootSceneNode();
   }

   XMLNodes = XMLRoot->FirstChildElement( "nodes" );

   // Read in the scene nodes
   if( XMLNodes )
   {
      processNode(XMLNodes->FirstChildElement( "node" ), pAttachNode, mPhysicsScene);
   }

   // Close the XML File
   delete XMLDoc;
}

void LevelXmlParser::processNode(TiXmlElement *XMLNode, SceneNode *pAttach, OgreBulletDynamics::DynamicsWorld* mPhysicsScene )
{
   TiXmlElement *XMLPosition, *XMLRotation, *XMLScale,  *XMLEntity, *XMLBillboardSet,  *XMLLight, *XMLUserData;
   while( XMLNode )
   {
      // Process the current node
      // Grab the name of the node
      String NodeName = XMLNode->Attribute("name");
      // First create the new scene node
      SceneNode* NewNode = pAttach->createChildSceneNode( m_sPrependNode + NodeName );
    // BEGIN: Physics Collision
        OgreBulletDynamics::RigidBody* tmpPhysicsBody;
        Ogre::String tmpPhysicsName;
        Ogre::String tmpPhysicsMeshFilename;
        Ogre::Vector3 tmpPhysicsVecScale;
        bool isPhysicsBody = false;
    // END: Physics Collision
      Ogre::Vector3 TempVec;
      Ogre::String TempValue;

      // Scale it. (Before I add it in my collision ! Later is too late. )
      XMLScale = XMLNode->FirstChildElement("scale");
      if( XMLScale ){
         TempValue = XMLScale->Attribute("x");
         TempVec.x = StringConverter::parseReal(TempValue);
         TempValue = XMLScale->Attribute("y");
         TempVec.y = StringConverter::parseReal(TempValue);
         TempValue = XMLScale->Attribute("z");
         TempVec.z = StringConverter::parseReal(TempValue);
         NewNode->setScale( TempVec );
         tmpPhysicsVecScale = TempVec;
         // Humm this is not the good thing to do for OgreNewt... Maybe the right way is to scale the mesh, not the node...
      }

      XMLLight = XMLNode->FirstChildElement( "light" );
      if( XMLLight )
         NewNode->attachObject( LoadLight( XMLLight, mSceneMgr ) );

      // Check for an Entity
      XMLEntity = XMLNode->FirstChildElement("entity");
      if( XMLEntity )
      {
         String EntityName, EntityMeshFilename;
         EntityName = XMLEntity->Attribute( "name" );
         EntityMeshFilename = XMLEntity->Attribute( "meshFile" );

        // BEGIN: Physics Collision
            tmpPhysicsName = EntityName;
            tmpPhysicsMeshFilename = EntityMeshFilename;
            isPhysicsBody = true;
        // END: Physics Collision
      }

      XMLBillboardSet = XMLNode->FirstChildElement( "billboardSet" );
      if( XMLBillboardSet )
      {
         String TempValue;

         BillboardSet* bSet = mSceneMgr->createBillboardSet( NewNode->getName() );

         BillboardType Type;
         TempValue = XMLBillboardSet->Attribute( "type" );
         if( TempValue == "orientedCommon" )
            Type = BBT_ORIENTED_COMMON;
         else if( TempValue == "orientedSelf" )
            Type = BBT_ORIENTED_SELF;
         else Type = BBT_POINT;

         BillboardOrigin Origin;
         TempValue = XMLBillboardSet->Attribute( "type" );
         if( TempValue == "bottom_left" )
            Origin = BBO_BOTTOM_LEFT;
         else if( TempValue == "bottom_center" )
            Origin = BBO_BOTTOM_CENTER;
         else if( TempValue == "bottomRight"  )
            Origin = BBO_BOTTOM_RIGHT;
         else if( TempValue == "left" )
            Origin = BBO_CENTER_LEFT;
         else if( TempValue == "right" )
            Origin = BBO_CENTER_RIGHT;
         else if( TempValue == "topLeft" )
            Origin = BBO_TOP_LEFT;
         else if( TempValue == "topCenter" )
            Origin = BBO_TOP_CENTER;
         else if( TempValue == "topRight" )
            Origin = BBO_TOP_RIGHT;
         else
            Origin = BBO_CENTER;

         bSet->setBillboardType( Type );
         bSet->setBillboardOrigin( Origin );


         TempValue = XMLBillboardSet->Attribute( "name" );
         bSet->setMaterialName( TempValue );

         int width, height;
         width = (int) StringConverter::parseReal( XMLBillboardSet->Attribute( "width" ) );
         height = (int) StringConverter::parseReal( XMLBillboardSet->Attribute( "height" ) );
         bSet->setDefaultDimensions( width, height );
         bSet->setVisible( true );
         NewNode->attachObject( bSet );

         TiXmlElement *XMLBillboard;

         XMLBillboard = XMLBillboardSet->FirstChildElement( "billboard" );

         while( XMLBillboard )
         {
            Billboard *b;
            // TempValue;
            TempVec = Vector3( 0, 0, 0 );
            ColourValue TempColour(1,1,1,1);

            XMLPosition = XMLBillboard->FirstChildElement( "position" );
            if( XMLPosition ){
               TempValue = XMLPosition->Attribute("x");
               TempVec.x = StringConverter::parseReal(TempValue);
               TempValue = XMLPosition->Attribute("y");
               TempVec.y = StringConverter::parseReal(TempValue);
               TempValue = XMLPosition->Attribute("z");
               TempVec.z = StringConverter::parseReal(TempValue);
            }

            TiXmlElement* XMLColour = XMLBillboard->FirstChildElement( "colourDiffuse" );
            if( XMLColour ){
               TempValue = XMLColour->Attribute("r");
               TempColour.r = StringConverter::parseReal(TempValue);
               TempValue = XMLColour->Attribute("g");
               TempColour.g = StringConverter::parseReal(TempValue);
               TempValue = XMLColour->Attribute("b");
               TempColour.b = StringConverter::parseReal(TempValue);
            }

            b = bSet->createBillboard( TempVec, TempColour);

            XMLBillboard = XMLBillboard->NextSiblingElement( "billboard" );
         }
      }

      // Now position it...
      Quaternion TempQuat;

      XMLPosition = XMLNode->FirstChildElement("position");
      if( XMLPosition ){
         TempValue = XMLPosition->Attribute("x");
         TempVec.x = StringConverter::parseReal(TempValue);
         TempValue = XMLPosition->Attribute("y");
         TempVec.y = StringConverter::parseReal(TempValue);
         TempValue = XMLPosition->Attribute("z");
         TempVec.z = StringConverter::parseReal(TempValue);
      }

      // Rotate it...
      XMLRotation = XMLNode->FirstChildElement("rotation");
      if( XMLRotation ){
         
         TempValue = XMLRotation->Attribute("qx");
         TempQuat.x = StringConverter::parseReal(TempValue);
         TempValue = XMLRotation->Attribute("qy");
         TempQuat.y = StringConverter::parseReal(TempValue);
         TempValue = XMLRotation->Attribute("qz");
         TempQuat.z = StringConverter::parseReal(TempValue);
         TempValue = XMLRotation->Attribute("qw");
         TempQuat.w = StringConverter::parseReal(TempValue);
         // BEGIN: Physics Collision
         if ( isPhysicsBody )
         {
             tmpPhysicsBody = Discover::ObjectManager::createSceneStatic(mSceneMgr, mPhysicsScene, tmpPhysicsName, tmpPhysicsMeshFilename, TempVec, TempQuat, 0.8);
         }
         // END: Physics Collision
         else
         {
            NewNode->setPosition( TempVec );
            NewNode->setOrientation( TempQuat );
         }
      }


      XMLUserData = XMLNode->FirstChildElement( "userData" );
      if ( XMLUserData )
      {
         TiXmlElement *XMLProperty;
         XMLProperty = XMLUserData->FirstChildElement("property");
         while ( XMLProperty )
         {
            String first = NewNode->getName();
            String second = XMLProperty->Attribute("name");
            String third = XMLProperty->Attribute("data");
            String type = XMLProperty->Attribute("type");
            nodeProperty newProp(first,second,third,type);
            nodeProperties.push_back(newProp);
            XMLProperty = XMLProperty->NextSiblingElement("property");
         }   
      }
      TiXmlElement * ChildXMLNode;
      ChildXMLNode = XMLNode->FirstChildElement( "node" );
      if(ChildXMLNode)
         processNode(ChildXMLNode, NewNode, mPhysicsScene);   // recurse to do all my children

      XMLNode = XMLNode->NextSiblingElement( "node" ); // process my next sibling
   }
}

String LevelXmlParser::getProperty(String ndNm, String prop)
{
   for ( unsigned int i = 0 ; i < nodeProperties.size(); i++ )
   {
      if ( nodeProperties[i].nodeName == ndNm && nodeProperties[i].propertyNm == prop )
      {
         return nodeProperties[i].valueName;
      }
   }
   return " ";
}