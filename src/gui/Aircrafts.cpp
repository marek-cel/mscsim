/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

#include <gui/Aircrafts.h>

#include <QFile>

#include <gui/gui_Defines.h>

#include <fdm/utils/fdm_Units.h>

#include <sim/Path.h>

////////////////////////////////////////////////////////////////////////////////

Aircrafts::Aircrafts()
{
    QFile file( Path::get( "gui/aircrafts.xml" ).c_str() );

    if ( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QDomDocument doc;

        doc.setContent( &file, false );

        QDomElement rootNode = doc.documentElement();

        if ( rootNode.tagName() == "aircrafts" )
        {
            QDomElement aircraftNode = rootNode.firstChildElement( "aircraft" );

            while ( !aircraftNode.isNull() )
            {
                parseAircraft( aircraftNode );
                aircraftNode = aircraftNode.nextSiblingElement( "aircraft" );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

Aircrafts::~Aircrafts() {}

////////////////////////////////////////////////////////////////////////////////

void Aircrafts::parseAircraft(const QDomElement &node )
{
    QDomElement nodeName = node.firstChildElement( "name" );
    QDomElement nodeType = node.firstChildElement( "type" );
    QDomElement nodeFile = node.firstChildElement( "file" );

    QDomElement nodeDistDef = node.firstChildElement( "distance_def" );
    QDomElement nodeDistMin = node.firstChildElement( "distance_min" );

    QDomElement nodeCameraOffset = node.firstChildElement( "camera_offset" );

    QDomElement nodeVne = node.firstChildElement( "vne" );

    if ( !nodeName.isNull() && !nodeType.isNull() && !nodeFile.isNull() && !nodeVne.isNull() )
    {
        Aircraft aircraft;

        aircraft.name = nodeName.text();
        aircraft.type = nodeType.text().toInt( NULL );
        aircraft.file = nodeFile.text();

        aircraft.distance_def = nodeDistDef.isNull() ? 5.0 : nodeDistDef.text().toFloat();
        aircraft.distance_min = nodeDistMin.isNull() ? 5.0 : nodeDistMin.text().toFloat();

        aircraft.offset_x = 0.0;
        aircraft.offset_y = 0.0;
        aircraft.offset_z = 0.0;

        if ( !nodeCameraOffset.isNull() )
        {
            QDomElement nodeOffsetX = nodeCameraOffset.firstChildElement( "x" );
            QDomElement nodeOffsetY = nodeCameraOffset.firstChildElement( "y" );
            QDomElement nodeOffsetZ = nodeCameraOffset.firstChildElement( "z" );

            aircraft.offset_x = nodeOffsetX.isNull() ? 0.0 : nodeOffsetX.text().toFloat();
            aircraft.offset_y = nodeOffsetY.isNull() ? 0.0 : nodeOffsetY.text().toFloat();
            aircraft.offset_z = nodeOffsetZ.isNull() ? 0.0 : nodeOffsetZ.text().toFloat();
        }

        aircraft.vfe = 0.0;

        QDomElement nodeVfe = node.firstChildElement( "vfe" );

        if ( !nodeVfe.isNull() )
        {
            aircraft.vfe = nodeVfe.text().toFloat();

            if ( nodeVfe.hasAttribute( "unit" ) )
            {
                std::string unit_name = nodeVfe.attributeNode( "unit" ).value().toStdString();
                fdm::Units::fptr converter = fdm::Units::getConverter( unit_name.c_str() );

                if ( converter )
                    aircraft.vfe = ( *converter )( aircraft.vfe );
            }
        }

        aircraft.vne = nodeVne.text().toFloat();

        if ( nodeVne.hasAttribute( "unit" ) )
        {
            std::string unit_name = nodeVne.attributeNode( "unit" ).value().toStdString();
            fdm::Units::fptr converter = fdm::Units::getConverter( unit_name.c_str() );

            if ( converter )
                aircraft.vne = ( *converter )( aircraft.vne );
        }

        parseAircraftAxes       ( node, aircraft.axes       );
        parseAircraftControls   ( node, aircraft.controls   );
        parseAircraftPropulsion ( node, aircraft.propulsion );
        parseAircraftMasses     ( node, aircraft.masses     );

        aircraft.mainRotorCoef = 1.0f;
        aircraft.tailRotorCoef = 1.0f;

        QDomElement nodeMainRotorCoef = node.firstChildElement( "main_rotor_coef" );
        QDomElement nodeTailRotorCoef = node.firstChildElement( "tail_rotor_coef" );

        if ( !nodeMainRotorCoef.isNull() )
        {
            aircraft.mainRotorCoef = nodeMainRotorCoef.text().toFloat();
        }

        if ( !nodeTailRotorCoef.isNull() )
        {
            aircraft.tailRotorCoef = nodeTailRotorCoef.text().toFloat();
        }

        _aircrafts.push_back( aircraft );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircrafts::parseAircraftAxes( const QDomElement &node, Axes &axes )
{
    axes.roll.curvature  = 0.0;
    axes.roll.deadzone   = 0.0;
    axes.roll.scale      = 1.0;

    axes.pitch.curvature = 0.0;
    axes.pitch.deadzone  = 0.0;
    axes.pitch.scale     = 1.0;

    axes.yaw.curvature   = 0.0;
    axes.yaw.deadzone    = 0.0;
    axes.yaw.scale       = 1.0;

    QDomElement nodeAxes = node.firstChildElement( "axes" );

    if ( !nodeAxes.isNull() )
    {
        QDomElement nodeRoll  = nodeAxes.firstChildElement( "roll"  );
        QDomElement nodePitch = nodeAxes.firstChildElement( "pitch" );
        QDomElement nodeYaw   = nodeAxes.firstChildElement( "yaw"   );

        parseAircraftAxis( nodeRoll  , axes.roll  );
        parseAircraftAxis( nodePitch , axes.pitch );
        parseAircraftAxis( nodeYaw   , axes.yaw   );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircrafts::parseAircraftAxis( const QDomElement &node, Axis &axis )
{
    if ( !node.isNull() )
    {
        QDomElement nodeCurvature = node.firstChildElement( "curvature" );
        QDomElement nodeDeadzone  = node.firstChildElement( "deadzone"  );
        QDomElement nodeScale     = node.firstChildElement( "scale"     );

        if ( !nodeCurvature .isNull() ) axis.curvature = nodeCurvature .text().toDouble();
        if ( !nodeDeadzone  .isNull() ) axis.deadzone  = nodeDeadzone  .text().toDouble();
        if ( !nodeScale     .isNull() ) axis.scale     = nodeScale     .text().toDouble();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircrafts::parseAircraftControls( const QDomElement &node, Controls &controls )
{
    controls.collective  = false;
    controls.landingGear = false;
    controls.flaps       = false;
    controls.throttle    = 0;
    controls.propeller   = 0;
    controls.mixture     = 0;
    controls.airbrake    = false;
    controls.spoilers    = false;

    QDomElement nodeControls = node.firstChildElement( "controls" );

    if ( !nodeControls.isNull() )
    {
        QDomElement nodeCollective  = nodeControls.firstChildElement( "collective"   );
        QDomElement nodeLandingGear = nodeControls.firstChildElement( "landing_gear" );
        QDomElement nodeFlaps       = nodeControls.firstChildElement( "flaps"        );
        QDomElement nodeThrottle    = nodeControls.firstChildElement( "throttle"     );
        QDomElement nodePropeller   = nodeControls.firstChildElement( "propeller"    );
        QDomElement nodeMixture     = nodeControls.firstChildElement( "mixture"      );
        QDomElement nodeAirbrake    = nodeControls.firstChildElement( "airbrake"     );
        QDomElement nodeSpoilers    = nodeControls.firstChildElement( "spoilers"     );

        controls.collective  = !nodeCollective.isNull();
        controls.landingGear = !nodeLandingGear.isNull();
        controls.airbrake    = !nodeAirbrake.isNull();
        controls.spoilers    = !nodeSpoilers.isNull();

        if ( !nodeFlaps.isNull() )
        {
            controls.flaps = true;

            QDomElement nodeNotch = nodeFlaps.firstChildElement( "notch" );

            while ( !nodeNotch.isNull() )
            {
                controls.notches.push_back( nodeNotch.text().toDouble() );

                nodeNotch = nodeNotch.nextSiblingElement( "notch" );
            }
        }

        if ( !nodeThrottle.isNull() )
        {
            controls.throttle = nodeThrottle.attributeNode( "count" ).value().toInt();
        }

        if ( !nodePropeller.isNull() )
        {
            controls.propeller = nodePropeller.attributeNode( "count" ).value().toInt();
        }

        if ( !nodeMixture.isNull() )
        {
            controls.mixture = nodeMixture.attributeNode( "count" ).value().toInt();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircrafts::parseAircraftPropulsion( const QDomElement &node, Propulsion &propulsion )
{
    propulsion.engines = 0;
    propulsion.ab   = false;
    propulsion.rpm  = false;
    propulsion.prop = false;
    propulsion.ng   = false;
    propulsion.n1   = false;
    propulsion.n2   = false;
    propulsion.trq  = false;
    propulsion.epr  = false;
    propulsion.map  = false;
    propulsion.egt  = false;
    propulsion.cht  = false;
    propulsion.tit  = false;
    propulsion.tot  = false;
    propulsion.itt  = false;

    QDomElement nodePropulsion = node.firstChildElement( "propulsion" );

    if ( !nodePropulsion.isNull() )
    {
        propulsion.engines = nodePropulsion.attributeNode( "engines" ).value().toInt();

        QDomElement nodeAB   = nodePropulsion.firstChildElement( "ab"   );
        QDomElement nodeRPM  = nodePropulsion.firstChildElement( "rpm"  );
        QDomElement nodePROP = nodePropulsion.firstChildElement( "prop" );
        QDomElement nodeNG   = nodePropulsion.firstChildElement( "ng"   );
        QDomElement nodeN1   = nodePropulsion.firstChildElement( "n1"   );
        QDomElement nodeN2   = nodePropulsion.firstChildElement( "n2"   );
        QDomElement nodeTRQ  = nodePropulsion.firstChildElement( "trq"  );
        QDomElement nodeEPR  = nodePropulsion.firstChildElement( "epr"  );
        QDomElement nodeMAP  = nodePropulsion.firstChildElement( "map"  );
        QDomElement nodeEGT  = nodePropulsion.firstChildElement( "egt"  );
        QDomElement nodeCHT  = nodePropulsion.firstChildElement( "cht"  );
        QDomElement nodeTIT  = nodePropulsion.firstChildElement( "tit"  );
        QDomElement nodeTOT  = nodePropulsion.firstChildElement( "tot"  );
        QDomElement nodeITT  = nodePropulsion.firstChildElement( "itt"  );

        propulsion.ab   = !nodeAB   .isNull();
        propulsion.rpm  = !nodeRPM  .isNull();
        propulsion.prop = !nodePROP .isNull();
        propulsion.ng   = !nodeNG   .isNull();
        propulsion.n1   = !nodeN1   .isNull();
        propulsion.n2   = !nodeN2   .isNull();
        propulsion.trq  = !nodeTRQ  .isNull();
        propulsion.epr  = !nodeEPR  .isNull();
        propulsion.map  = !nodeMAP  .isNull();
        propulsion.egt  = !nodeEGT  .isNull();
        propulsion.cht  = !nodeCHT  .isNull();
        propulsion.tit  = !nodeTIT  .isNull();
        propulsion.tot  = !nodeTOT  .isNull();
        propulsion.itt  = !nodeITT  .isNull();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircrafts::parseAircraftMasses( const QDomElement &node, Masses &masses )
{
    QDomElement nodeMasses = node.firstChildElement( "masses" );

    if ( !nodeMasses.isNull() )
    {
        QDomElement nodeEmpty = nodeMasses.firstChildElement( "empty" );
        QDomElement nodeMTOW  = nodeMasses.firstChildElement( "mtow" );

        masses.empty = ( nodeEmpty .isNull() ) ? 0.0 : nodeEmpty .text().toDouble();
        masses.mtow  = ( nodeMTOW  .isNull() ) ? 0.0 : nodeMTOW  .text().toDouble();

        QDomElement nodePilot1 = nodeMasses.firstChildElement( "pilot_1" );
        QDomElement nodePilot2 = nodeMasses.firstChildElement( "pilot_2" );

        QDomElement nodeFuel1 = nodeMasses.firstChildElement( "fuel_tank_1" );
        QDomElement nodeFuel2 = nodeMasses.firstChildElement( "fuel_tank_2" );
        QDomElement nodeFuel3 = nodeMasses.firstChildElement( "fuel_tank_3" );
        QDomElement nodeFuel4 = nodeMasses.firstChildElement( "fuel_tank_4" );
        QDomElement nodeFuel5 = nodeMasses.firstChildElement( "fuel_tank_5" );
        QDomElement nodeFuel6 = nodeMasses.firstChildElement( "fuel_tank_6" );
        QDomElement nodeFuel7 = nodeMasses.firstChildElement( "fuel_tank_7" );
        QDomElement nodeFuel8 = nodeMasses.firstChildElement( "fuel_tank_8" );

        QDomElement nodeCabin = nodeMasses.firstChildElement( "cabin" );
        QDomElement nodeTrunk = nodeMasses.firstChildElement( "cargo_trunk" );

        parseAircraftMass( nodePilot1 , masses.pilot_1 );
        parseAircraftMass( nodePilot2 , masses.pilot_2 );

        parseAircraftMass( nodeFuel1 , masses.fuel_tank_1 );
        parseAircraftMass( nodeFuel2 , masses.fuel_tank_2 );
        parseAircraftMass( nodeFuel3 , masses.fuel_tank_3 );
        parseAircraftMass( nodeFuel4 , masses.fuel_tank_4 );
        parseAircraftMass( nodeFuel5 , masses.fuel_tank_5 );
        parseAircraftMass( nodeFuel6 , masses.fuel_tank_6 );
        parseAircraftMass( nodeFuel7 , masses.fuel_tank_7 );
        parseAircraftMass( nodeFuel8 , masses.fuel_tank_8 );

        parseAircraftMass( nodeCabin, masses.cabin );
        parseAircraftMass( nodeTrunk, masses.trunk );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircrafts::parseAircraftMass( QDomElement &massNode, Masses::Mass &mass )
{
    mass.enabled = false;

    mass.max = 0.0;
    mass.def = 0.0;

    if ( !massNode.isNull() )
    {
        mass.enabled = true;

        mass.name = massNode.attributeNode( "name" ).value();

        mass.max = massNode.attributeNode( "maximum" ).value().toDouble();
        mass.def = massNode.attributeNode( "default" ).value().toDouble();
    }
}
