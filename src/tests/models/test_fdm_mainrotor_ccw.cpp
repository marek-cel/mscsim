   _beta_0 = ( gamma / 2.0 ) * ( _b3 * lambda / 3.0 + _b3 * p * mu / ( 6.0 * _omega ) + _b4 * _theta_0 / 4.0 + _b2 * _theta_0 * mu2 / 4.0 )
           - a_z * _sb / ( _ib * omega2 );

   beta_1c_cwas = 2.0 * mu * ( lambda + 4.0 * _b * _theta_0 / 3.0 ) / ( mu2 / 2.0 - _b2 )
           + ( _b4 * p / _omega - 16.0 * q / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 - _b2 ) );

   beta_1s_cwas = -4.0 * _beta_0 * mu * _b / ( mu2 / 2.0 + _b2 ) / 3.0
           + ( _b4 * q / _omega + 16.0 * p / ( gamma * _omega ) ) / ( _b2 * ( mu2 / 2.0 + _b2 ) );
