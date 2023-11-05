<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
*** Will do.
-->


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/NucleaPeon/qt5menugen">
    <!--<img src="icons/qjsonify-512x512.png" alt="Logo" width="80" height="80">-->
  </a>

  <h3 align="center">qt5menugen</h3>

  <p align="center">
    Library that builds out Menus and Toolbars for Windows, Linux and OS X using a JSON file
    <br />
    <br />
    <a href="https://github.com/NucleaPeon/qt5menugen/issues">Report Bug</a>
    ·
    <a href="https://github.com/NucleaPeon/qt5menugen/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
<!--
[![qt5menugen on Linux][product-screenshot-linux]](https://github.com/NucleaPeon/qt5menugen)
[![qt5menugen on OS X Snow Leopard][product-screenshot-osx]](https://github.com/NucleaPeon/QJsonify)
[![qt5menugen on Windows 7][product-screenshot-win7]](https://github.com/NucleaPeon/QJsonify)
-->
``qt5menugen``: Generate Mac OSX, Windows and Linux -specific toolbars and menus via a json file. Instead of having many QMenu and QAction declarations in your qt5 c++ header files, this allows initialization and retrieval of QActions in one statement. Library comes with an example project to see how it is used.

### Oxygen Icon Licensing

This project includes Oxygen Icons <a href="https://techbase.kde.org/Projects/Oxygen/Licensing">https://techbase.kde.org/Projects/Oxygen/Licensing</a> which are licensed under the LGPL v3 (see LICENSE-LGPL3 file). All files under this license are located in the ``example/ExampleWindow/*.png`` directory.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

This project is built on a minimum Qt5 version of Qt 5.3.2. For OS X 10.6.8, you can download the Qt5 binary at my <a href="https://github.com/NucleaPeon/qt5-sle/releases/tag/v5.3.2">other repository</a>.

It is also tested using Qt5.15 on Linux amd64.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

Clone the repo:

```sh
    git clone https://github.com/NucleaPeon/qt5menugen.git
```


### Prerequisites

You will need:
* git
* cmake (for CMake builds)
* clang >= 3 OR g++ >= 4.2 OR msvc 
* make (OS X and Linux)
* Qt5 >= 5.3.2 (Though 5.3.1 **may** also be acceptable)
* For Windows 7:
	* Visual Studio with MSBuild.exe on your ``PATH`` (I'm using VS 2013)
	* Windows Driver Version kit (I'm using 7.1.0)
	* Git Bash is useful for cli builds
	* Ensure that Qt5_DIR environmental variable is set to something like ``C:\Qt\Qt5.3.1\5.3\msvc2013_opengl`` or ``C:\Qt\Qt5.3.2\5.3\msvc2013_64_opengl``
	* Ensure your path to windeployqt is on ``PATH`` (``C:\Qt\Qt5.3.1\5.3\msvc2013_opengl\Bin``)


### Installation

**Note** I recommend calling ``make -jN`` where ``N`` is the number of cores of your cpu.

<hr>

* Using cmake on Linux or Mac:

  ```sh
      cd build/
      cmake -DCMAKE_INSTALL_PREFIX=/usr/ ..
      make -j2
      sudo make install
  ```

* For Windows 7 on Visual Studio 2013


TODO
<!--
	```sh
	cd build
	cmake ..
	# Define Debug or Release for build type. "//" to specify an option is required for git bash, but only "/" for cmd.exe or powershell.
	MSBuild.exe qJsonify.sln //p:Configuration="Debug"
	windeployqt Debug/QJsonify.exe
	```
Compress the Release/ or Debug/ directory and distribute.
  -->
	

## Usage

Once the library is installed, you can access it via qmake by including this code:

```cmake
  target_link_libraries(${PROJECT_NAME} Qt5::Widgets qt5menugen)
```

<!-- ROADMAP -->
## Roadmap

- [ ] Improve documentation
- [ ] Add/release static versions or include instructions and qt5 5.3.2 statically compiled binaries
- [ ] Create a debian release package
- [ ] Create a gentoo guru ebuild

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

Please remember to avoid any c++11 code and any Qt 5.4.0+ introduced functions should have Qt 5.3.2 versions defined for Snow Leopard compatibility.

If you have a suggestion that would make this better, either create an issue with your idea(s) or fork the repo and create a pull request.

Don't forget to give the project a star! Thanks again!

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the GPL v2.0 License. See `LICENSE` for more information.

I am purposefully using a more liberal GNU License for libraries to encourage adoption.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

You can email me at <initial.dann@gmail.com>.

[https://github.com/NucleaPeon](https://github.com/NucleaPeon)

Thanks to the <a href="https://github.com/othneildrew/Best-README-Template">https://github.com/othneildrew/Best-README-Template</a> project for helping me get started on this README.md.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[product-screenshot-linux]: images/qjsonify-linux.png
[product-screenshot-osx]: images/qjsonify-osx.png
[product-screenshot-win7]: images/qjsonify-win7.png
