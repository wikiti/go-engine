# GO Engine


## 1. Resumen ##
*Go Engine* es un motor gráfico multiplataforma para poder utilizar como base a la hora de realizar videojuegos. El nombre proviene de la orientación del proyecto (*GameObject Engine*), basado en entidades y componentes.

<div align="center">
![](doc/logo.png)
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
- [SOIL](http://www.opengl.org/wiki/Image_Libraries "SOIL") (Carga de imágenes)
- [Glew32](http://glew.sourceforge.net/ "glew32") (Extensión de gráficos) 
- [OpenAL](http://kcat.strangesoft.net/openal.html "OpenAL") (Sonido)
- [ASSIMP 3.0.1270](http://assimp.sourceforge.net/ "ASSIMP") (Modelos 3D, etc)
- [GLM](http://glm.g-truc.net/ "GL Maths") (Matemática de gráficos 3D)
- [SDL2](http://www.libsdl.org/ "SDL2")
	- [SDL_image 2.0](http://www.libsdl.org/projects/SDL_image/ "SDL_image 2.0")
	- [SDL_mixer 2.0	](http://www.libsdl.org/projects/SDL_mixer/ "SDL_mixer 2.0")
	- [SDL_ttf 2.0](http://www.libsdl.org/projects/SDL_ttf/ "SDL_ttf 2.0") <span style="color: red; font-weight: bold;"><code>-- Sin usar</code></span>
	- [SDL_net 2.0](	http://www.libsdl.org/projects/SDL_net/ "SDL_net 2.0") <span style="color: red; font-weight: bold;"><code>-- Sin usar</code></span>
- [Qt](http://qt-project.org/ "Qt") (Interfaz gráfica del editor) <span style="color: red; font-weight: bold;"><code>-- Sin usar</code></span>
- [Bullet Engine](http://bulletphysics.org/wordpress/ "Bullet Engine") (Física) <span style="color: red; font-weight: bold;"><code>-- Sin usar</code></span>


## 5. Documentación ##

La página de documentación del proyecto puede encontrarse en la carpeta "/doc" del repositorio, o bien accediendo a [http://wikiti.github.io/go-engine](http://wikiti.github.io/go-engine).

## 6. Construcción del proyecto ##
El usuario debe programar en el directorio *user/*, que será usado para compilar el proyecto en conjunto.

En un futuro, se hará que el motor gráfico sea un ejecutable, y los ficheros del usuario sean unas librerías dll (*shared libs*), que será una solución algo mas limpia, pero más problemática.

Para construir el proyecto, se utilizará la herramienta [CMake](http://www.cmake.org/) para el preparado de las dependencias y la compilación (módulos cmake *Find&lt;módulo&gt;.cmake*).

Simplemente, basta con usar la herramienta cmake dentro del directorio ***build/***, junto con la *toolchain* deseada, y luego ejecutar la herramienta make.

	> cd build/
	> cmake .. -G "<Toolchain aquí>"
	... Corregir errores, relanzar cmake, etc.
	> make


Y se generá el ejecutable ***goengine***, que deberá ser ejecutado:

	> cd ..
	> ./build/goengine

Básicamente, el ejecutable se debe lanzar desde la **carpeta raíz del proyecto**, si no, no encontrará los archivos en la carpeta **data/**.
Por comodidad, se puede copiar el ejecutable a la carpeta raíz, o crear una carpeta bin y poner el ejecutable dentro, además de crear un acceso directo para ejecutarlo desde la raíz.


Para saber la lista de *toolchains* disponibles, basta con usar:

	> cmake --help

Por defecto, si no se usa la opción *-G*, se usará la *toolchain* predeterminada.

Recomiendo commpilar el proyecto en ***Windows***, con ***MiNGW Makefiles***

<span style="color: red; font-weight: bold;">Nota:</span> ***MSYS Makefiles*** para *Windows* dará problemas, ya que no enlazará correctamente la librería *libSDL2main.a*. 

## 7. Autores ##

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
</table>
<!-- Fin tabla -->


## 8. Enlaces de interés ##

- Página VIII CUSL: [http://www.concursosoftwarelibre.org/1314/proyectos/20](http://www.concursosoftwarelibre.org/1314/proyectos/20)
- Blog Wordpress: [http://goengine.wordpress.com/](http://goengine.wordpress.com/)
- Página en ZehnGames: [http://www.zehngames.com/comunidad/tema/wip-motor-grafico-multiplataforma-go-engine/](http://www.zehngames.com/comunidad/tema/wip-motor-grafico-multiplataforma-go-engine/)