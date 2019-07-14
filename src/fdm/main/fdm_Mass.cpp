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

#include <fdm/main/fdm_Mass.h>
#include <fdm/main/fdm_Aircraft.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/xml/fdm_XmlUtils.h>

////////////////////////////////////////////////////////////////////////////////

using namespace fdm;

////////////////////////////////////////////////////////////////////////////////

Mass::Mass( const Aircraft* aircraft ) :
    m_aircraft ( aircraft ),

    m_mass_e ( 0.0 ),
    m_mass_t ( 0.0 )
{}

////////////////////////////////////////////////////////////////////////////////

Mass::~Mass() {}

////////////////////////////////////////////////////////////////////////////////

void Mass::init()
{
    update();
}

////////////////////////////////////////////////////////////////////////////////

void Mass::readData( XmlNode &dataNode )
{
    if ( dataNode.isValid() )
    {
        int result = FDM_SUCCESS;

        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_mass_e   , "empty_mass"     );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_it_e_bas , "inertia_tensor" );
        if ( result == FDM_SUCCESS ) result = XmlUtils::read( dataNode, m_cm_e_bas , "center_of_mass" );

        XmlNode varMassNode = dataNode.getFirstChildElement( "variable_mass" );

        while ( result == FDM_SUCCESS && varMassNode.isValid() )
        {
            VarMass varMass;

            varMass.input = 0;

            std::string name = varMassNode.getAttribute( "name" );

            if ( result == FDM_SUCCESS ) result = XmlUtils::read( varMassNode, varMass.mass_max , "mass_max"    );
            if ( result == FDM_SUCCESS ) result = XmlUtils::read( varMassNode, varMass.r_bas    , "coordinates" );

            if ( result == FDM_SUCCESS )
            {
                std::pair<Masses::iterator,bool> temp =
                        m_masses.insert( std::pair<std::string,VarMass>( name, varMass ) );

                if ( temp.second != true )
                {
                    result = FDM_FAILURE;
                }
            }

            if ( result != FDM_SUCCESS )
            {
                Exception e;

                e.setType( Exception::FileReadingError );
                e.setInfo( "Reading XML file failed. " + XmlUtils::getErrorInfo( varMassNode ) );

                FDM_THROW( e );
            }

            varMassNode = varMassNode.getNextSiblingElement( "variable_mass" );
        }

        if ( result != FDM_SUCCESS )
        {
            Exception e;

            e.setType( Exception::FileReadingError );
            e.setInfo( "Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

            FDM_THROW( e );
        }
    }
    else
    {
        Exception e;

        e.setType( Exception::FileReadingError );
        e.setInfo( "Reading XML file failed. " + XmlUtils::getErrorInfo( dataNode ) );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Mass::computeForceAndMoment()
{   
    m_for_bas = m_mass_t * m_aircraft->getGrav_BAS();
    m_mom_bas = m_cm_t_bas ^ m_for_bas;

    if ( !m_for_bas.isValid() || !m_mom_bas.isValid() )
    {
        Exception e;

        e.setType( Exception::UnexpectedNaN );
        e.setInfo( "ERROR! NaN detected in the mass model." );

        FDM_THROW( e );
    }
}

////////////////////////////////////////////////////////////////////////////////

void Mass::update()
{
    m_mass_t   = m_mass_e;
    m_st_t_bas = m_mass_e * m_cm_e_bas;
    m_it_t_bas = m_it_e_bas;

    for ( Masses::iterator it = m_masses.begin(); it != m_masses.end(); it++ )
    {
        VarMass &vm = (*it).second;

        if ( vm.input )
            vm.mass = Misc::satur( 0.0, vm.mass_max, *(vm.input) );
        else
            vm.mass = Misc::satur( 0.0, vm.mass_max, 0.0 );

        addVariableMass( vm );
    }

    m_cm_t_bas = m_st_t_bas / m_mass_t;

    //std::cout << __FILE__ << "(" << __LINE__ << ") CG: " << m_cm_t_bas.toString() << std::endl;
    //std::cout << __FILE__ << "(" << __LINE__ << ") mt: " << m_mass_t << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

Matrix6x6 Mass::getInertiaMatrix() const
{
    Matrix6x6 mi_bas;

    mi_bas(0,0) =  m_mass_t;
    mi_bas(0,1) =  0.0;
    mi_bas(0,2) =  0.0;
    mi_bas(0,3) =  0.0;
    mi_bas(0,4) =  m_st_t_bas.z();
    mi_bas(0,5) = -m_st_t_bas.y();

    mi_bas(1,0) =  0.0;
    mi_bas(1,1) =  m_mass_t;
    mi_bas(1,2) =  0.0;
    mi_bas(1,3) = -m_st_t_bas.z();
    mi_bas(1,4) =  0.0;
    mi_bas(1,5) =  m_st_t_bas.x();

    mi_bas(2,0) =  0.0;
    mi_bas(2,1) =  0.0;
    mi_bas(2,2) =  m_mass_t;
    mi_bas(2,3) =  m_st_t_bas.y();
    mi_bas(2,4) = -m_st_t_bas.x();
    mi_bas(2,5) =  0.0;

    mi_bas(3,0) =  0.0;
    mi_bas(3,1) = -m_st_t_bas.z();
    mi_bas(3,2) =  m_st_t_bas.y();
    mi_bas(3,3) =  m_it_t_bas(i_x,i_x);
    mi_bas(3,4) =  m_it_t_bas(i_x,i_y);
    mi_bas(3,5) =  m_it_t_bas(i_x,i_z);

    mi_bas(4,0) =  m_st_t_bas.z();
    mi_bas(4,1) =  0.0;
    mi_bas(4,2) = -m_st_t_bas.x();
    mi_bas(4,3) =  m_it_t_bas(i_y,i_x);
    mi_bas(4,4) =  m_it_t_bas(i_y,i_y);
    mi_bas(4,5) =  m_it_t_bas(i_y,i_z);

    mi_bas(5,0) = -m_st_t_bas.y();
    mi_bas(5,1) =  m_st_t_bas.x();
    mi_bas(5,2) =  0.0;
    mi_bas(5,3) =  m_it_t_bas(i_z,i_x);
    mi_bas(5,4) =  m_it_t_bas(i_z,i_y);
    mi_bas(5,5) =  m_it_t_bas(i_z,i_z);

    return mi_bas;
}

////////////////////////////////////////////////////////////////////////////////

void Mass::addVariableMass( const VarMass &varMass )
{
    // Taylor J.: Classical Mechanics, p.411
    m_mass_t   += varMass.mass;
    m_st_t_bas += varMass.mass * varMass.r_bas;

    double r_x2 = varMass.r_bas.x() * varMass.r_bas.x();
    double r_y2 = varMass.r_bas.y() * varMass.r_bas.y();
    double r_z2 = varMass.r_bas.z() * varMass.r_bas.z();

    double d_it_xy = varMass.mass * varMass.r_bas.x() * varMass.r_bas.y();
    double d_it_xz = varMass.mass * varMass.r_bas.x() * varMass.r_bas.z();
    double d_it_yz = varMass.mass * varMass.r_bas.y() * varMass.r_bas.z();

    m_it_t_bas(i_x,i_x) += varMass.mass * ( r_y2 + r_z2 );
    m_it_t_bas(i_x,i_y) -= d_it_xy;
    m_it_t_bas(i_x,i_z) -= d_it_xz;

    m_it_t_bas(i_y,i_x) -= d_it_xy;
    m_it_t_bas(i_y,i_y) += varMass.mass * ( r_x2 + r_z2 );
    m_it_t_bas(i_y,i_z) -= d_it_yz;

    m_it_t_bas(i_z,i_x) -= d_it_xz;
    m_it_t_bas(i_z,i_y) -= d_it_yz;
    m_it_t_bas(i_z,i_z) += varMass.mass * ( r_x2 + r_y2 );
}

////////////////////////////////////////////////////////////////////////////////

Mass::VarMass* Mass::getVariableMassByName( const std::string &name )
{
    Masses::iterator it = m_masses.find( name );

    if ( it != m_masses.end() )
    {
        return &(it->second);
    }

    return 0;
}
