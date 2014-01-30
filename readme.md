# GO Engine


## 1. Resumen ##
*Go Engine* es un motor gráfico multiplataforma para poder utilizar como base a la hora de realizar videojuegos. El nombre proviene de la orientación del proyecto (*GameObject Engine*), basado en entidades y componentes.

<div align="center">
![](assets/logo.png)
<div align="left">


## 2. Descripción detallada ##
En este proyecto, trataremos de implementar un motor de videojuegos multiplataforma (*Windows, Linux y Mac*) que será utilizado como API por los usuarios para realizar sus propios videojuegos.
El sistema dispondrá de una simple interfaz gráfica que permitirá a los usuarios trabajar de una forma más cómoda e intuitiva.
Dentro de las especificaciones del motor, podemos mencionar las siguientes:

- Gráficos 3D con *OpenGL*
- Sonido 3D con *OpenAL* (u otras)
- Capacidad de red (networking)
- Motor físico con *Bullet Engine*
- Interfaz gráfica de usuario sencilla (o GUI)

Para ello, se usará un paradigma del tipo orientado a objetos, en este caso, orientado a objetos y componentes (véase [éste](http://www.genbetadev.com/programacion-de-videojuegos/diseno-de-videojuegos-orientado-a-entidades-y-componentes "éste") enlace ).

Además, se desarrollarán una serie de sistemas que trabajarán con dichos objetos (como mostrarlos por pantalla, actualizar su posición, etc).

Se definirán 3 partes en el proyecto bien claras y definidas:

- **Engine**: Parte principal del motor (llámese kernel, núcleo, etc), conteniendo las definiciones de los objetos de juego, componentes y sistemas.
- **Usuario**: Apartado en el que irá el código del usuario.
- **Editor**: Interfaz gráfica para que el usuario pueda trabajar cómodamente.


## 3. Implementación ##
***C++*** implementado en [**Mingw**](http://www.mingw.org/) (principalmente), *GCC* o cualquier compilador libre de *C++*


## 4. Librerías y dependencias ##
- [OpenGL32](http://www.opengl.org/ "OpenGL32") (Gráficos) 
- [Glew32](http://glew.sourceforge.net/ "glew32") (Extensión de gráficos) 
- [OpenAL](http://kcat.strangesoft.net/openal.html "OpenAL") (Sonido)
- [Qt](http://qt-project.org/ "Qt") (Interfaz gráfica del editor)
- [SDL2](http://www.libsdl.org/ "SDL2")
	- [SDL_image 2.0](http://www.libsdl.org/projects/SDL_image/ "SDL_image 2.0")
	- [SDL_ttf 2.0](http://www.libsdl.org/projects/SDL_ttf/ "SDL_ttf 2.0")
	- [SDL_mixer 2.0	](http://www.libsdl.org/projects/SDL_mixer/ "SDL_mixer 2.0")
	- [SDL_net 2.0](	http://www.libsdl.org/projects/SDL_net/ "SDL_net 2.0")
- [SOIL](http://www.opengl.org/wiki/Image_Libraries "SOIL") (Imágenes)
- [Bullet Engine](http://bulletphysics.org/wordpress/ "Bullet Engine") (Física)
- [ASSIMP](http://assimp.sourceforge.net/ "ASSIMP") (Modelos 3D, etc)
- [GLM](http://glm.g-truc.net/ "GL Maths") (Matemática de gráficos 3D)
- [Boost](http://www.boost.org/ "Boost") (Serialization) (Planteado eliminar su uso)
- Otras menores (ya se mencionarán cuando se decidan)


## 5. Autores ##

Este proyecto ha sido desarrollado, en conjunto, por:

<!-- Tabla -->
<table cellspacing="0">
  <tr  style="background-color: #E3E3E3;">
    <td> <b>Avatar</b> </td>
    <td> <b>Nombre</b> </td>
    <td> <b>Nickname</b> </td>
	<td> <b>Correo electrónico</b> </td>
  </tr>
  <tr style="background-color: #FFFFFF;">
    <td> <img width="64"src="http://imageshack.us/a/img209/6782/parrotav.png"/> </td>
    <td> Daniel Herzog Cruz </td>
    <td> <b>Wikiti</b> </td>
	<td> <a href="mailto:wikiti.doghound@gmail.com"> wikiti.doghound@gmail.com</a> </td>
  </tr>
  <tr style="background-color: #FFFFFF;">
    <td> <img width="64"src="http://media.steampowered.com/steamcommunity/public/images/avatars/a8/a85132cd5fbfd5f510012342a3074ef613003332_full.jpg"/> </td>
    <td> Laura Fariña Rodríguez </td>
    <td> <b>Kiarapoke</b> </td>
	<td> <a href="mailto:kiarapoke@gmail.com"> kiarapoke@gmail.com</a> </td>
  </tr>
  <tr style="background-color: #FFFFFF;">
    <td> <img width="64"src="http://media.steampowered.com/steamcommunity/public/images/avatars/6d/6d11192b1c41f360c6763f55d926436b112c207e_full.jpg"/> </td>
    <td> Eliasib Jesús García Martín </td>
    <td> <b>Eliasib13</b> </td>
	<td> <a href="mailto:eliasib.lv.12@gmail.com"> eliasib.lv.12@gmail.com</a> </td>
  </tr>
</table>
<!-- Fin tabla -->


## 6. Enlaces de interés ##

- Página VIII CUSL: [http://www.concursosoftwarelibre.org/1314/proyectos/20](http://www.concursosoftwarelibre.org/1314/proyectos/20)
- Blog Wordpress: [http://goengine.wordpress.com/](http://goengine.wordpress.com/)