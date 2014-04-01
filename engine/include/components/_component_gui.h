/**
 * @file
 * @brief Fichero que incluye la clase CComponent_GUI_Texture.
 */

#ifndef __COMPONENT_GUI_TEXTURE_H_
#define __COMPONENT_GUI_TEXTURE_H_

#include "_globals.h"
#include "components/_component.h"

/** @addtogroup Componentes */
/*@{*/

/**
 * @brief Componente textura GUI
 *
 * Un componente interesante relacionado con el render es aquel que pueda mostrar una interfaz gráfica de usuario por medio de
 * una “capa” mostrada en nuestra pantalla. Se podrán mostrar texturas (implementado en este componente) y texto (no implementado aún)
 * de tal forma que se ajusten de manera cómoda a la pantalla. Este es el caso del componente "textura".
 *
 * @image html components/gui/gui.png
 *
 * @warning La imagen superior está mal. Los valores de "position" para la componente x y la componente y, van desde 0 hasta el ancho máximo y el alto máximo de la pantalla respectivamente.
 * @warning El ancho también va especificado en píxeles, no en porcentaje. Véase los atributos de la clase para entenderlo.
 *
 * Una componente de este estilo se usará para mostrar una imagen o textura sobre la pantalla, por encima de todo,
 * con el fin de hacer una especie de interfaz. Esto puede ser útil para mostrar marcas de agua, barras de vida, números, indicadores, etc.
 *
 * El funcionamiento de este componente tiene 2 partes: una nueva funcionalidad en el Render y otra parte en el componente en sí:
 *
 * En el Render, se ha definido una nueva cámara invisible para el usuario, llamada __GUI_Camera, encargada de renderizar el
 * contenido de los componentes GUI. Dicha cámara se caracteriza por tener una proyección del tipo ortográfica
 * en el origen de nuestro mundo. Por tanto, se renderizarán de la siguiente forma:
 *
 * 1. Desactivar el buffer de profundidad (GL_DEPTH_BUFFER).
 * 2. Ajustar el viewport de la pantalla.
 * 3. Cargar matriz ortográfica en la matriz GL_PROJECTION.
 * 4. Para cada componente GUI hallado en el primer recorrido de objetos (main camera):
 *    5. Renderizar el componente GUI.
 *
 * Lo mencionado se hará de manera automática por el sistema CSystem_Render.
 *
 * En el componente GUI se guarda un color, una textura o material, un ancho y un alto (valores entre 0 y el ancho/alto de pantalla)
 * y un offset dado por píxeles. Y ahora decimos… ¿Cómo se guarda la posición de la textura en la pantalla?
 * Para ello, usamos el componente Transform, que nos serviará para guarda la posición en pantalla
 * (X e Y con valores entre 0 y y el ancho/alto de la pantalla, siendo el 0 la izquierda, el ancho de la pantalla la derecha
 * para la X, y el 0 abajo y el alto de la pantalla arriba para la Y, y un valor de Z que nos dirá qué elementos GUI van delante
 * y cuales van detrás (uno con un valor Z menor que otro se dibujará después que otro), la escala (como se expande la textura o texto)
 * y la rotación (como se rota el elemento con respecto a su centro). Para el renderizado, se hace lo siguiente:
 *
 * 1. Trasladarse a la posición dada por el componente Transform del objeto (position).
 * 2. Rotar por un ángulo dado por el componente Transform del objeto (angle).
 * 3. Escalar por un factor dado por el componente Transform del objeto (scale).
 * 4. Seleccionar textura y color.
 * 5. Dibujar aplicando offsets.
 *
 * Lo mencionado se hará de manera automática por el componente.
 *
 * Por ejemplo, podemos crear una textura para colocar sobre la pantalla de la siguiente manera:
 @code
 CGameObject* gui_example = gGameObjects.Add("gui_example");                     // Crear objeto.

 gui_example->GUITexture()->texture_name = "textura1";                           // Cambiar textura a mostrar.
 gui_example->GUITexture()->width = gui_example->GUITexture()->height = 100.f;   // Cambiar ancho y alto a 100 píxeles.
 gui_example->Transform()->position(100.f, 100.f, 0.f);                          // Cambiar posición a la posición 100, 100, empezando desde abajo a la izquierda.
 gui_example->GUITexture()->pixel_offset_x = 2;                                  // Cambiar el offset, en píxeles, a 2.
 gui_example->GUITexture()->color.a = 0.5f;                                      // Cambiar transparencia a la mitad.
 gui_example->GUITexture()->color.g = gui_example->GUITexture()->color.b = 0.f;  // Cambiar color a rojo (componentes verde y azul a 0).
 @endcode
 *
 * @see CSystem_Render
 */
class CComponent_GUI_Texture: public CComponent
{
  friend class CSystem_Render;
  friend class CGameObject;

  public:
    std::string texture_name; /**< Nombre de la imagen a mostrar como *skybox*. No es el nombre del fichero, sino del recurso. @see CSystem_Resources @see CResource_Texture */

    GLint pixel_offset_x; /**< Offset, en píxeles, del eje x. */
    GLint pixel_offset_y; /**< Offset, en píxeles, del eje y. */

    GLfloat width;  /**< Ancho de la imagen, expresada en píxeles. Dependerá del ancho de la pantalla, y deberá ser gestionado a mano (mayor control). */
    GLfloat height; /**< Alto de la imagen, expresada en píxeles. Dependerá del alto de la pantalla, y deberá ser gestionado a mano (mayor control). */

    colorf_t color; /**< Color a aplicar sobre la textura. Afectado incluso por la transparencia. Se trata de una aplicación aditiva (simplemente colocar el color encima).*/

    GLfloat color_apply_force; /**< Fuerza con la que se aplica el color. Si vale 1, la textura será inundada por el color. Si vale 0, se añadirá el color a la textura (adición).*/

  private:
    static GLuint m_GUITextureVBOVertices;
    static GLuint m_GUITextureVBOTexCoords;
    static GLuint m_GUITextureVAO;

    static int GetID() { return Components::gui_texture; }
    static bool InitRenderVBO();
    static void CloseRenderVBO();

    void UpdateVBO();

    void parseDebug(std::string command);
    void printDebug();

    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);

  public:
    /**
     * @brief Constructor vacío.
     */
    CComponent_GUI_Texture(){};

    /** @brief Constructor con objeto asociado.
     *
     * Asocia al objeto pasado como argumento el componente creado. Además, inicializa los atributos de la clase a unos ciertos valores:
     *
     @code
     texture_name = "";

     pixel_offset_x = pixel_offset_y = 0;
     width = height = 1.f;

     color_apply_force = 0.f;

     color(1.f, 1.f, 1.f, 1.f); // Blanco opaco
     @endcode
     *
     * @param gameObject Objeto que guardará el componente.
     */
    CComponent_GUI_Texture(CGameObject* gameObject);

    /**
     * @brief Destructor.
     *
     * Destruye el componente.
     */
    ~CComponent_GUI_Texture();

    /**
     * @brief Comprobar colisión
     *
     * Dado un punto de la pantalla, expresado como una coordenada *x,y*, en píxeles, se comprobará
     * si el píxel pertenece al rectángulo que representa a la textura gui.
     *
     * @warning No se tendrá en cuenta la trasparencia o la rotación, por lo que se considerará siempre que la textura es un **rectángulo completo no rotado**.
     *
     * @param pixel_x Coordenada x del píxel.
     * @param pixel_y Coordenada y del píxel.
     * @return Devuelve true si el pixel esta dentro de la textura, false en caso contrario.
     */
    bool HitTest(GLfloat pixel_x, GLfloat pixel_y);
};

/*@}*/

#endif /* __COMPONENT_GUI_TEXTURE_H_ */
