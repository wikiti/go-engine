/**
 * @file
 * @brief Fichero que incluye la clase CComponent_Mesh_Render.
 */

#ifndef __COMPONENT_MESH_RENDER_H_
#define __COMPONENT_MESH_RENDER_H_

#include "_globals.h"
#include "components/_component.h"

/** @addtogroup Componentes */
/*@{*/

/**
 * @brief Componente textura GUI
 *
 * Un componente muy útil y necesario para la representación gráfica son los modelos o
 * objetos gráficos.
 *
 * @image html crate.png
 *
 * Un modelo es un elemento externo (un objeto 3d de Blender, Maya, 3DMax...)
 * que ha sido cargado en el motor gráfico como un recurso.
 *
 * Por ejemplo, supóngase que se ha creado un objeto desde un programa externo, en un
 * formato soportado por la librería de *Assimp* (.3ds, .obj, etc.). Sencillamente,
 * se deberá cargar el recurso con el sistema CSystem_Resources, y utilizando el nombre del
 * recurso de modelo como el objetivo a dibujar (CComponent_Mesh_Render::mesh_name). Se extraerá
 * del componente de transformación la posición, orientación y escala para dibujar el modelo.
 *
 * Además, se debe añadir un nombre de una textura y un shader (se usará uno por defecto si no se especifica)
 * para dibujar el modelo.
 *
 * Por tanto, si se quisiera dibujar una caja, habría que:
 *
 * 1. Obtener un modelo (crearlo o descargarlo) desde un programa externo.
 * 2. Cargar el modelo en el motor gráfico (mediante ficheros .rc de recursos, véase CSystem_Resources).
 * 3. Asociar el componente *Mesh Render* a un objeto, junto con el nombre del modelo, su textura, y su shader.
 * 4. Dejar que el sistema dibuje el objeto según crea.
 *
 * Por ejemplo, para usar un modelo, usamos:
 *
 * **Fichero *recursos.rc**:*
 @code
 # Esto línea es un comentario.
 # Formato de resource:
 # tipo: nombre_recurso ruta_al_fichero: [argumentos]

 mesh: mdl_hada1 data/resources/models/faerie.md2:
 # Un "mesh" no es más que un modelo.
 @endcode
 *
 * Código fuente del programa:
 @code
 CGameObject* hada1 = gGameObjects.Add("hada1"); // Crear objeto

 // Cambiar atributos de transformación.
 hada1->Transform()->SetScale(0.1f, 0.1f, 0.1f);            // Reducir tamaño
 hada1->Transform()->Translate(0.f, 0.f, 10.f);             // Trasladar
 hada1->Transform()->Rotate(0.f, 90.f, 0.f);                // Rotar sobre el eje Y (guiñada o *yaw*)

 hada1->MeshRender()->mesh_name = "mdl_hada1";              // Especificar el modelo
 hada1->MeshRender()->material_name = "texture_mdl_hada1";  // Especificar la textura
 hada1->MeshRender()->color(1.0, 0.9f, 0.9f, 1.f);          // Aplicar el color.
 @endcode
 *
 * @bug No funciona, hay que arreglar un problema con la carga de modelos de modelos de *Assimp*.
 *
 * @see CSystem_Resources
 * @see CSystem_Shader_Manager
 * @see CResource_Texture
 * @see CResource_Model
 * @see http://assimp.sourceforge.net/
 * @see http://es.wikipedia.org/wiki/Shader
 */
class CComponent_Mesh_Render: public CComponent
{
  friend class CSystem_Render;
  friend class CGameObject;

  public:
    std::string mesh_name;      /**< Nombre del recurso-modelo a usar. @see CSystem_Resources @see CResource_Model */
    std::string material_name;  /**< Nombre del recurso-textura a usar. Se usará el mapa UV del modelo para mapear la textura. @see CSystem_Resources @see CResource_Texture */
    std::string shader_name;    /**< Nombre del shader o *programa* a usar. @see CSystem_Shader_Manager */

    //vector<string> materials;
    // Guardar colores en un vector, o algo por el estilo...
    colorf_t color;             /**< Color a aplicar sobre el modelo. Afectado incluso por la transparencia. Se trata de una aplicación aditiva (simplemente colocar el color encima).*/
    GLfloat color_apply_force;  /**< Fuerza con la que se aplica el color. Si vale 1, el modelo será inundada por el color. Si vale 0, se añadirá el color al modelo (adición).*/

    // Puede ser útil para
    function_t before_render;   /**< Callback a ejecutar antes de renderizar el modelo. Se usará por si se quieren hacer modificaciones a bajo nivel (OpenGL). Si es NULL, no se ejecutará nada. */
    function_t after_render;    /**< Callback a ejecutar después de renderizar el modelo. Se usará por si se quieren hacer modificaciones a bajo nivel (OpenGL). Si es NULL, no se ejecutará nada. */

    // Modo difuso, etc etc

  private:
    static int GetID() { return Components::mesh_render; }

    void parseDebug(std::string command);
    void printDebug();

  public:
    /** @brief Constructor vacío. */
    CComponent_Mesh_Render(){};
    /** @brief Constructor con objeto asociado.
     *
     * Asocia al objeto pasado como argumento el componente creado. Además, inicializa los atributos de la clase a unos ciertos valores:
     *
     @code
     mesh_name = material_name = "";
     shader_name = "__textureShader";
     before_render = after_render = NULL;

     color(1.f, 1.f, 1.f, 1.f);
     color_apply_force = 0.f;
     @endcode
     *
     * @param gameObject Objeto que guardará el componente.
     */
    CComponent_Mesh_Render(CGameObject* gameObject);
    /** @brief Destructor.
     * Destruye el componente. */
    ~CComponent_Mesh_Render();

  protected:
    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
};

/*@}*/

#endif /* __COMPONENT_MESH_RENDER_H_ */
