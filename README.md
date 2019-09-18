<!--
*** Thanks for checking out this README Template. If you have a suggestion that would
*** make this better please fork the repo and create a pull request or simple open
*** an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
-->


<!-- PROJECT SHIELDS -->
[![Build Status][build-shield]]()
[![Contributors][contributors-shield]]()
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/othneildrew/Best-README-Template">
    <img src="https://avatars0.githubusercontent.com/u/54409822?s=400&u=2f84e4804a8f69a090045e7da931d7ec1c47a45e&v=4" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">IoT Blockchain Endpoint</h3>

  <p align="center">
    Un proyecto sencillo para el seguimiento y control de bolsas de sangre usando IoT / RFID
    <br />
    <a href="https://github.com/right-donor/rfid-blood"><strong>Explora la documentacion</strong></a>
    <br />
    <br />
    <a href="https://github.com/right-donor/rfid-blood">Ve el demo</a>
    ·
    <a href="https://github.com/right-donor/rfid-blood/issues">Reporta un Bug</a>
    ·
    <a href="https://github.com/right-donor/rfid-blood/issues">Pide una mejora</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Tabla de Contenidos

* [Acerca del Proyecto](#about-the-project)
  * [Construído con](#built-with)
* [Empezar](#getting-started)
  * [Pre-Requisitos](#prerequisites)
  * [Instalación](#installation)
* [Uso](#usage)
* [Contribución](#contributing)
* [Licencia](#license)
* [Contacto](#contact)
* [Agradecimientos](#acknowledgements)



<!-- ABOUT THE PROJECT -->
## Acerca del Proyecto

[![Product Name Screen Shot][product-screenshot]](https://www.rightdonor.org)

Este es uno de los multiples proyectos que forman parte de Right Donor. La utilidad de este proyecto es la de:
* Permitirle a los medicos obtener el ID de un Tag de una bolsa de sangre.
* Permitirle a los transportistas llevar un control del sitio donde se encuentra una bolsa de sangre.


### Construido con:
Este proyecto fue realizado usando los siguientes componentes:

* [NodeMCU v1.0](https://www.nodemcu.com/index_cn.html)
* [Buzzer 12mm](https://www.steren.com.mx/mini-buzzer-de-4-khz-de-1-5-a-16-vcc-con-se-al-de-tono-constante-de-72-db.html)
* [RC522](https://randomnerdtutorials.com/security-access-using-mfrc522-rfid-reader-with-arduino/)


<!-- GETTING STARTED -->
## Empezar

### Prerequisites

Obtener todos los componentes presentados anteriormente.

### Instalación

1. Clona el repositorio
```sh
git clone https:://github.com/right-donor/rfid-blood.git
```
2. Abre Arduino IDE
3. Sigue los pasos contenidos en [la siguiente liga](https://www.teachmemicro.com/intro-nodemcu-arduino) para usar NodeMCU.
4. Realiza la conexión de los componentes como aparece en el diagrama principal.
5. Modifica el apartado de SSID y Password para conectarse a la red que tu desees
```c++ 
const char *ssid = "XXXXXXXX";
const char *pass = "XXXXXXXX";
```
6. Presiona CMD+R en MacOS o CTRL+R en Windows/Linux

<!-- USAGE EXAMPLES -->
## Uso
Este proyecto fue creado para ser usado en conjunto con el proyecto de Right Donor para la mejora de la cultura de donación de sangre en México. El código incluído en este repositorio puede ser modificado para:
* Crear un servidor de Autenticación Local
* Hacer sonar un Buzzer
* Crear un Wi-Fi Jammer

<!-- CONTRIBUTING -->
## Contribución

Cualquier contribución que se haga a este proyecto será **grandemente apreciada**.

1. Crea un Fork del proyecto
2. Crea tu propio Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Haz un commit de tus cambios (`git commit -m 'Add some AmazingFeature`)
4. Realiza un Push al Branch (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request



<!-- LICENSE -->
## License

Distribuído bajo la licencia del MIT. Véase `LICENSE` para más información


<!-- CONTACT -->
## Contact

Fernando Martín García Del Angel - [@martntn](https://twitter.com/martntn) - a01334390@tec.mx
Project Link: [https://github.com/right-donor/rfid-blood](https://github.com/right-donor/rfid-blood)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [GitHub Emoji Cheat Sheet](https://www.webpagefx.com/tools/emoji-cheat-sheet)
* [Img Shields](https://shields.io)
* [Choose an Open Source License](https://choosealicense.com)



<!-- MARKDOWN LINKS & IMAGES -->
[build-shield]: https://img.shields.io/badge/build-passing-brightgreen.svg?style=flat-square
[contributors-shield]: https://img.shields.io/badge/contributors-1-orange.svg?style=flat-square
[license-shield]: https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square
[license-url]: https://choosealicense.com/licenses/mit
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/martingarciadelangel
[product-screenshot]: https://raw.githubusercontent.com/right-donor/rfid-blood/master/right_donoror_bb.png
