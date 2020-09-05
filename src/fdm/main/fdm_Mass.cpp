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

#include <fdm/main/fdm_Mass.h>
#include <fdm/main/fdm_Aircraft.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Mass::Mass( const Aircraft* aircraft, DataNode *rootNode ) :
    Module ( aircraft, rootNode ),

    _mass_e ( 0.0 ),
    _mass_t ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Mass::~Mass() {}

////////////////////////////////////////////////////////////////////////////////

void Mass::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _mass_e     , "empty_mass"     );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _i_e_bas    , "inertia_tensor" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, _r_cm_e_bas , "center_of_mass" );

        XmlNode varMassNode = dataNode.getFirstChildElement( "variable_mass" );

        while ( result == FDM_SUCCESS && varMassNode.isValid() )
        {
            VarMass varMass;

            std::string name  = varMassNode.getAttribute( "name"  );
            std::string input = varMassNode.getAttribute( "input" );

            varMass.dr_input = getDataRef( input );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( varMassNode, varMass.mass_max , "mass_max"    );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( varMassNode, varMass.r_bas    , "coordinates" );

            if ( result == FDM_SUCCESS )
            {
                std::pair<Masses::iterator,bool> temp =
                        _masses.insert( std::pair<std::string,VarMass>( name, varMass ) );

                if ( temp.second != true )
                {
                    result = FDM_FAILURE;
                }
            }

            if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, varMassNode );

            varMassNode = varMassNode.getNextSiblingElement( "variable_mass" );
        }

        if ( result != FDM_SUCCESS ) XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
    else
    {
        XmlUtils::throwError( __FILE__, __LINE__, dataNode );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Mass::initialize()
{
    update();
}

////////////////////////////////////////////////////////////////////////////////

void Mass::computeForceAndMoment()
{   
    _for_bas = _mass_t * _aircraft->getGrav_BAS();
    _mom_bas = _r_cm_t_bas % _for_bas;

    if ( !_for_bas.isValid() || !_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "NaN detected in the mass model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Mass::update()
{
    _mass_t   = _mass_e;
    _s_t_bas = _mass_e * _r_cm_e_bas;
    _i_t_bas = _i_e_bas;

    for ( Masses::iterator it = _masses.begin(); it != _masses.end(); ++it )
    {
        VarMass &vm = (*it).second;

        if ( vm.dr_input.isValid() )
            vm.mass = Misc::satur( 0.0, vm.mass_max, vm.dr_input.getValue() );
        else
            vm.mass = Misc::satur( 0.0, vm.mass_max, 0.0 );

        addVariableMass( vm );
    }

    _r_cm_t_bas = _s_t_bas / _mass_t;
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6 Mass::getInertiaMatrix() const
{
    Matrix6x6 mi_bas;

    mi_bas(0,0) =  _mass_t;
    mi_bas(0,1) =  0.0;
    mi_bas(0,2) =  0.0;
    mi_bas(0,3) =  0.0;
    mi_bas(0,4) =  _s_t_bas.z();
    mi_bas(0,5) = -_s_t_bas.y();

    mi_bas(1,0) =  0.0;
    mi_bas(1,1) =  _mass_t;
    mi_bas(1,2) =  0.0;
    mi_bas(1,3) = -_s_t_bas.z();
    mi_bas(1,4) =  0.0;
    mi_bas(1,5) =  _s_t_bas.x();

    mi_bas(2,0) =  0.0;
    mi_bas(2,1) =  0.0;
    mi_bas(2,2) =  _mass_t;
    mi_bas(2,3) =  _s_t_bas.y();
    mi_bas(2,4) = -_s_t_bas.x();
    mi_bas(2,5) =  0.0;

    mi_bas(3,0) =  0.0;
    mi_bas(3,1) = -_s_t_bas.z();
    mi_bas(3,2) =  _s_t_bas.y();
    mi_bas(3,3) =  _i_t_bas.xx();
    mi_bas(3,4) =  _i_t_bas.xy();
    mi_bas(3,5) =  _i_t_bas.xz();

    mi_bas(4,0) =  _s_t_bas.z();
    mi_bas(4,1) =  0.0;
    mi_bas(4,2) = -_s_t_bas.x();
    mi_bas(4,3) =  _i_t_bas.yx();
    mi_bas(4,4) =  _i_t_bas.yy();
    mi_bas(4,5) =  _i_t_bas.yz();

    mi_bas(5,0) = -_s_t_bas.y();
    mi_bas(5,1) =  _s_t_bas.x();
    mi_bas(5,2) =  0.0;
    mi_bas(5,3) =  _i_t_bas.zx();
    mi_bas(5,4) =  _i_t_bas.zy();
    mi_bas(5,5) =  _i_t_bas.zz();

    return mi_bas;
}

////////////////////////////////////////////////////////////////////////////////

void Mass::addVariableMass( const VarMass &varMass )
{
    // Taylor J.: Classical Mechanics, p.411
    _mass_t  += varMass.mass;
    _s_t_bas += varMass.mass * varMass.r_bas;

    double r_x2 = varMass.r_bas.x() * varMass.r_bas.x();
    double r_y2 = varMass.r_bas.y() * varMass.r_bas.y();
    double r_z2 = varMass.r_bas.z() * varMass.r_bas.z();

    double d_it_xy = varMass.mass * varMass.r_bas.x() * varMass.r_bas.y();
    double d_it_xz = varMass.mass * varMass.r_bas.x() * varMass.r_bas.z();
    double d_it_yz = varMass.mass * varMass.r_bas.y() * varMass.r_bas.z();

    _i_t_bas.xx() += varMass.mass * ( r_y2 + r_z2 );
    _i_t_bas.xy() -= d_it_xy;
    _i_t_bas.xz() -= d_it_xz;

    _i_t_bas.yx() -= d_it_xy;
    _i_t_bas.yy() += varMass.mass * ( r_x2 + r_z2 );
    _i_t_bas.yz() -= d_it_yz;

    _i_t_bas.zx() -= d_it_xz;
    _i_t_bas.zy() -= d_it_yz;
    _i_t_bas.zz() += varMass.mass * ( r_x2 + r_y2 );
}

////////////////////////////////////////////////////////////////////////////////

Mass::VarMass* Mass::getVariableMassByName( const char *name )
{
    Masses::iterator it = _masses.find( name );

    if ( it != _masses.end() )
    {
        return &(it->second);
    }

    return 0;
}
