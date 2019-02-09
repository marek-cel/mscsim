/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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

////////////////////////////////////////////////////////////////////////////////

Aircrafts* Aircrafts::m_instance = 0;

////////////////////////////////////////////////////////////////////////////////

Aircrafts::Aircrafts()
{
    QFile file( "data/gui/aircrafts.xml" );

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

    if ( !nodeName.isNull() && !nodeType.isNull() && !nodeFile.isNull() )
    {
        Aircraft aircraft;

        aircraft.name = nodeName.text();
        aircraft.type = nodeType.text().toInt( NULL, 16 );
        aircraft.file = nodeFile.text();

        parseAircraftControls( node, aircraft.controls );
        parseAircraftPropulsion( node, aircraft.propulsion );
        parseAircraftMasses( node, aircraft.masses );

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

        m_aircrafts.push_back( aircraft );
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
        controls.flaps       = !nodeFlaps.isNull();
        controls.airbrake    = !nodeAirbrake.isNull();
        controls.spoilers    = !nodeSpoilers.isNull();

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
    propulsion.rpm  = false;
    propulsion.prop = false;
    propulsion.ng   = false;
    propulsion.n1   = false;
    propulsion.n2   = false;
    propulsion.trq  = false;
    propulsion.epr  = false;
    propulsion.map  = false;
    propulsion.egt  = false;
    propulsion.tot  = false;
    propulsion.itt  = false;

    QDomElement nodePropulsion = node.firstChildElement( "propulsion" );

    if ( !nodePropulsion.isNull() )
    {
        propulsion.engines = nodePropulsion.attributeNode( "engines" ).value().toInt();

        QDomElement nodeRPM  = nodePropulsion.firstChildElement( "rpm"  );
        QDomElement nodePROP = nodePropulsion.firstChildElement( "prop" );
        QDomElement nodeNG   = nodePropulsion.firstChildElement( "ng"   );
        QDomElement nodeN1   = nodePropulsion.firstChildElement( "n1"   );
        QDomElement nodeN2   = nodePropulsion.firstChildElement( "n2"   );
        QDomElement nodeTRQ  = nodePropulsion.firstChildElement( "trq"  );
        QDomElement nodeEPR  = nodePropulsion.firstChildElement( "epr"  );
        QDomElement nodeMAP  = nodePropulsion.firstChildElement( "map"  );
        QDomElement nodeEGT  = nodePropulsion.firstChildElement( "egt"  );
        QDomElement nodeTOT  = nodePropulsion.firstChildElement( "tot"  );
        QDomElement nodeITT  = nodePropulsion.firstChildElement( "itt"  );

        propulsion.rpm  = !nodeRPM.isNull();
        propulsion.prop = !nodePROP.isNull();
        propulsion.ng   = !nodeNG.isNull();
        propulsion.n1   = !nodeN1.isNull();
        propulsion.n2   = !nodeN2.isNull();
        propulsion.trq  = !nodeTRQ.isNull();
        propulsion.epr  = !nodeEPR.isNull();
        propulsion.map  = !nodeMAP.isNull();
        propulsion.egt  = !nodeEGT.isNull();
        propulsion.tot  = !nodeTOT.isNull();
        propulsion.itt  = !nodeITT.isNull();
    }
}

////////////////////////////////////////////////////////////////////////////////

void Aircrafts::parseAircraftMasses( const QDomElement &node, Masses &masses )
{
    QDomElement nodeMasses = node.firstChildElement( "masses" );

    if ( !nodeMasses.isNull() )
    {
        QDomElement nodePilot  = nodeMasses.firstChildElement( "pilot"   );
        QDomElement nodePilotR = nodeMasses.firstChildElement( "pilot_l" );
        QDomElement nodePilotL = nodeMasses.firstChildElement( "pilot_r" );
        QDomElement nodePilotF = nodeMasses.firstChildElement( "pilot_f" );
        QDomElement nodePilotA = nodeMasses.firstChildElement( "pilot_a" );

        QDomElement nodeFuel  = nodeMasses.firstChildElement( "fuel"   );
        QDomElement nodeFuelR = nodeMasses.firstChildElement( "fuel_l" );
        QDomElement nodeFuelL = nodeMasses.firstChildElement( "fuel_r" );
        QDomElement nodeFuelF = nodeMasses.firstChildElement( "fuel_f" );
        QDomElement nodeFuelA = nodeMasses.firstChildElement( "fuel_a" );

        QDomElement nodeCabin = nodeMasses.firstChildElement( "cabin" );
        QDomElement nodeTrunk = nodeMasses.firstChildElement( "trunk" );

        parseAircraftMass( nodePilot  , masses.pilot  );
        parseAircraftMass( nodePilotL , masses.pilotL );
        parseAircraftMass( nodePilotR , masses.pilotR );
        parseAircraftMass( nodePilotF , masses.pilotF );
        parseAircraftMass( nodePilotA , masses.pilotA );

        parseAircraftMass( nodeFuel  , masses.fuel  );
        parseAircraftMass( nodeFuelL , masses.fuelL );
        parseAircraftMass( nodeFuelR , masses.fuelR );
        parseAircraftMass( nodeFuelF , masses.fuelF );
        parseAircraftMass( nodeFuelA , masses.fuelA );

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

        mass.max = massNode.attributeNode( "maximum" ).value().toDouble();
        mass.def = massNode.attributeNode( "default" ).value().toDouble();
    }
}
