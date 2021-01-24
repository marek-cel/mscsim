# mscsim
Flight simulation software.

## Installation

### Ubuntu 20.04 LTS / LinuxMint 20.x

Ubuntu Linux deb package is provided.

Additional data packages dependencies are required:

* [mscsim-data](https://github.com/marek-cel/mscsim-data)
* [mscsim-data-terrain](https://github.com/marek-cel/mscsim-data-terrain)

### Windows

Windows installer is provided. It contains all necessary data.

## Building from source

To build MScSim from source run CMake in src directory.

Additional library dependencies have to be provided, including:

* libxml2
* OpenAL and ALUT
* OpenScneGraph
* Qt5

On Ubuntu based operating systems those dependencies can be installed with the following command:

```sudo apt-get install libalut-dev libopenal-dev libopenscenegraph-dev libqt5opengl5-dev libqt5svg5-dev libqt5x11extras5-dev libqwt-qt5-dev libxml2-dev libxss-dev qtbase5-dev```

## Main features

High fidelity flight dynamics model based on available wind tunnel data and/or [CFD](https://en.wikipedia.org/wiki/Computational_fluid_dynamics) simulations including following aircraft types:

* [AW101](https://en.wikipedia.org/wiki/AgustaWestland_AW101) - medium helicopter
* [C-130](https://en.wikipedia.org/wiki/Lockheed_C-130_Hercules) - turboprop transport aircraft
* [Cessna 172](https://en.wikipedia.org/wiki/Cessna_172) - general aviation aircraft
* [F-16](https://en.wikipedia.org/wiki/General_Dynamics_F-16_Fighting_Falcon) - 4th generation jet fighter
* [F-35A](https://en.wikipedia.org/wiki/Lockheed_Martin_F-35_Lightning_II) - 5th generation jet fighter
* [Mustang Mk III](https://en.wikipedia.org/wiki/North_American_P-51_Mustang) - World War II era fighter
* [PW-5](https://en.wikipedia.org/wiki/PW-5) - World Class glider
* [R44](https://en.wikipedia.org/wiki/Robinson_R44) - light helicopter
* [UH-60](https://en.wikipedia.org/wiki/Sikorsky_UH-60_Black_Hawk) - medium helicopter

![AW101](screenshot_cfd.jpg)

![AW101](screenshot_aw101.jpg)

Electronic Flight Instrument System and Head-up Display common for all aircraft types. Built-in map with navigational radio aids, airports, etc..

![MScSim - GUI](screenshot_gui.jpg)

High resolution photorealistic scenery based on aerial imagery of Oahu island, low resolution scenery based on satellite imagery of the whole World.

![Scenery - Oahu](screenshot_oahu.jpg)

![Scenery - PHNL](screenshot_phnl.jpg)

Accurate celestial bodies position with almost 1000 visible stars.

![Stars](screenshot_stars.jpg)

Weather phenomena, such as clouds and fog.

![Clouds](screenshot_clouds.jpg)

![Fog](screenshot_fog.jpg)

## See also

* [mscsim-cfd](https://github.com/marek-cel/mscsim-cfd)
* [mscsim-data](https://github.com/marek-cel/mscsim-data)
* [mscsim-data-terrain](https://github.com/marek-cel/mscsim-data-terrain)
* [mscsim-docs](https://github.com/marek-cel/mscsim-docs)
* [mscsim-tools](https://github.com/marek-cel/mscsim-tools)

Visit [http://marekcel.pl/mscsim](http://marekcel.pl/mscsim) for more information.
