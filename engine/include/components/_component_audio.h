/**
 * @file
 * @brief Fichero que incluye la clase CComponent_Audio.
 */

#ifndef __COMPONENT_AUDIO_H_
#define __COMPONENT_AUDIO_H_

#include "_globals.h"
#include "components/_component.h"

#include "systems/_resource.h"

/** @addtogroup Componentes */
/*@{*/

/**
 * @brief Componente de audio.
 *
 * Una vez definido el gestor de sonido CSystem_Mixer, se pueden definir los componentes que van a representar fuentes de sonido dentro del juego.
 *
 * @image html components/audio_source/as.jpg
 *
 * Las principales características de un sonido son las siguientes:
 *
 * - Volumen
 * - Silenciado/No silenciado
 * - Pista de música o no (no es sonido 3D, sino mono).
 * - Pista tipo bucle o no (repetición continua).
 * - Empezar o no a sonar nada más cargar la estancia.
 * - Afección o no por el tiempo (modifica la frecuencia de manera interna dependiendo del tiempo)
 * - Reproducir o no en todas partes.
 * - Distancia máxima de reproducción (mínimo volumen por degradación).
 * - Distancia mínima de reproducción (máximo volumen por degradación).
 * - Las operaciones básicas para realizar con este componente son:
 * - Cambiar el sonido (fuente, recurso).
 * - Reproducir el sonido de manera estática (Play(), requiere bindear con Bind() el sonido antes de usarlo).
 *     Es más eficiente para casos en los que se usará el sonido varias veces, de manera continua.
 *     Nota: Un sonido que ha sido bindeado debe ser desbindeado (UnBind() de manera manual, ya que el recurso permanecerá ocupado hasta su liberación).
 * - Reproducir el sonido una única vez (PlayOneShot() y variantes, no requieren bindear).
 *     Es más eficiente para casos en los que se usará el sonido pocas veces, de manera esporádica.
 * - Reproducir en un determinado punto.
 * - Pausar, parar, rebobinar, etc.
 * - Bindear a un buffer del sistema CSystem_Mixer, y desbindear el buffer ocupado.
 *
 * Para entender las solicitudes de buffer con Bind() y UnBind(),
 * simplemente basta con pensar que un sonido no es más que una estructura de datos sencilla,
 * sin gestión de sonido. Para que funcione, una entidad externa (CSystem_Mixer), debe ser capaz de proporcionar los
 * medios necesarios para reproducir el sonido deseado. Para ello, proporciona una serie de buffers o "sources" para
 * almacenar sonidos. Si se pide registrar un buffer con Bind(), el sistema guardará el sonido en el buffer para un uso
 * cómodo y rápido, evitando esperas por el movimiento de sonido entre buffers.
 * Así, es el usuario el que decide como gestionar el sonido, de manera que, manualmente, deba liberar los recursos que quiera (con UnBind()).
 * No obstante, un sonido bindeado sólo se prodrá reproducir una vez cada vez, haciendo imposible reproducir un sonido varias veces (como disparos consecutivos).
 * Por ejemplo, podemos usar esto:
 *
 @code
 // ...
 // Iniciando el objeto desde la parte del usuario
 go1->AudioSource()->SetSound("sonido_bang"); // Cargado desde un fichero rc, véase CSystem_Resources
 go1->AudioSource()->Bind();
 // ...
 // Usando el componente en una función de gestión de eventos, lo que sea
 // ...
 go1->AudioSource()->Play();
 // ...
 // En otro momento
 // ...
 go1->AudioSource()->Pause();
 // ...
 // Luego...
 go1->AudioSource()->Play();
 // ...
 // Etc
 @endcode
 *
 * Por otra parte, las acciones del tipo CComponent_Audio_Source::PlayOneShot() no requieren del uso de CComponent_Audio_Source::Bind().
 * Esto reside en que, de manera automática, se solicita un buffer temporal en el sistema CSystem_Mixer, y una vez acabado,
 * se libera el recurso de manera automática. Por desgracia, este tipo de sonidos no se pueden modificar de manera dinámica,
 * por lo que no se verán afectados por cambio de sonido, u otros elementos. Esto sí que permite repetir sonidos de manera continua (disparos, etc).
 * Se recomienda usarlo para sonidos cortos. Por ejemplo, podemos usar esto:
 *
 @code
 // ...
 // Iniciando el objeto desde la parte del usuario
 go1->AudioSource()->SetSound("sonido_bang"); // Cargado desde un fichero rc, véase CSystem_Resources
 // ...
 // Usando el componente en una función de gestión de eventos, lo que sea
 // ...
 go1->AudioSource()->PlayOneShot();
 // ...
 // Etc
 @endcode
 *
 */
class CComponent_Audio_Source: public CComponent
{
  friend class CSystem_Mixer;
  friend class CGameObject;
  friend class CSystem_Render;

  protected:
    bool playing, paused;
    ALuint source_attached;

    CResource_Sound* sound;

    void Setup();

  public:
    ALfloat pitch;  /**< Frecuencia del sonido. */
    ALfloat volume; /**< Volumen del sonido. */

    bool mute;                 /**< ¿Está silenciado el sonido?. */
    bool music;                /**< ¿Es del tipo música (mono, sin sonido 3D)?. */
    bool loop;                 /**< ¿Se repite cada vez que acaba?. */
    bool start_playing;        /**< ¿Debe empezar reproduciéndose?. */
    bool affected_by_time;     /**< ¿Debe empezar reproduciéndose?. */
    bool everywhere;           /**< ¿Se reproducirá en todas partes?. */

    ALfloat max_distance;      /**< Máxima distancia de escucha (sonido con mínimo volumen por degradación a esta distancia). */
    ALfloat min_distance;      /**< Mínima distancia de escucha (sonido con máximo volumen por degradación a esta distancia). */
    /* That'll be alDistanceModel(AL_LINEAR_DISTANCE) or better alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED), which will turn distant sources off entirely.
    Read chapters 3.4.3 and 3.4.4 here. You also have to set the parameters in the formulas listed there (AL_MAX_DISTANCE and AL_REFERENCE_DISTANCE) for each source (alSourcef).
    If the parameters don't make sense, attenuation is turned off. If the sound is stereo, likewise (stereo is just that... stereo -- only mono sounds are 3D). */

  private:
    static int GetID() { return Components::audio_source; }

    void parseDebug(std::string command);
    void printDebug();

  public:
    /**
     * @brief Constructor vacío.
     */
    CComponent_Audio_Source(){};
    /** @brief Constructor con objeto asociado.
     *
     * Asocia al objeto pasado como argumento el componente creado. Además, inicializa los atributos de la clase a unos ciertos valores:
     *
     @code
     playing = paused = false;
     source_attached = 0;

     sound = NULL;
     pitch = volume = 1.f;

     mute = loop = start_playing = music = false;
     affected_by_time = true;

     everywhere = false;

     max_distance = 20.f;
     min_distance = 5.f;
     @endcode
     *
     * @param gameObject Objeto que guardará el componente.
     */
    CComponent_Audio_Source(CGameObject* gameObject);
    /**
     * @brief Destructor.
     *
     * Libera el recurso ocupado por el componente del sistema CSystem_Mixer, en caso de que no haya sido liberado.
     */
    ~CComponent_Audio_Source();

    /**
     * @brief Cambiar el sonido.
     *
     * Dado un nombre de un recurso de sonido, se intentará cambiar el recurso por un sonido dado. Si el sonido no existe, se almacenará NULL de manera interna, y se tomará como que no se ha asociado ningún sonido.
     *
     * @param name Nombre del sonido a almacenar. No es el nombre del fichero, sino del recurso.
     * @see CSystem_Resources
     * @see CResource_Sound
     */
    void SetSound(std::string name)
    {
      sound = gSystem_Resources.GetSound(name);
    }

    /**
     * Reproducir un sonido de manera estática.
     *
     * Se reproducirá el sonido en la posición, orientación y bajo la velocidad (sin usar) del objeto actual, revisando el componente CComponent_Transform del objeto.
     * En cada iteración, se actualizará el sonido, por lo que se verá afectado por el sonido, el volumen, la posición, y otros parámetros que se hayan definido.
     * Si no se ha bindeado, no sonará  nada, y se mostrará un mensaje de error por consola.
     *
     * @warning Se debe invocar al menos una vez a la función Bind() para poder reservar un recurso para reproducir el sonido.
     * @warning Posteriormente, al acabar de usar el sonido, se deberá llamar a la función CComponent_Audio_Source::UnBind().
     * @warning Sólo se podrá reproducir un sonido cada vez desde este componente. Para usar varios, véase PlayOneShot().
     *
     * @see Bind()
     * @see UnBind()
     */
    void Play();
    /**
     * Reproducir un sonido de manera dinámica.
     *
     * Se reproducirá el sonido una vez como un "one shot". Esto significa que el sonido será reproducido
     * una vez en el punto actual, dado por el componente CComponent_Transform del objeto, siendo invariante a nuevas modificaciones del volumen, frecuencia, etc.
     * Tampoco se podrá pausar, parar o rebobinar.
     *
     * Se podrá llamar varias veces seguidas, que serán reproducidos cada uno como un OneShot. Ideales para disparos y explosiones.
     *
     * @warning Existe un límite de OneShots definidos por la variable entera de juego __SOUND_NUMBER_SOURCES_ONESHOT
     * @warning No será necesario llamar a las funciones Bind() y UnBind() para usar este tipo de sonidos.
     */
    void PlayOneShot();
    /**
     * Reproducir un sonido de manera dinámica, con una posición dada.
     *
     * Funciona de manera similar a PlayOneShot(), pero en este caso se especificará la posición en la que se escuchará el sonido.
     * @param pos Vector de la posición del sonido.
     *
     * @see PlayOneShot()
     */
    void PlayOneShotAt(vector3f pos);
    /**
     * Reproducir un sonido de manera dinámica, con una posición dada.
     *
     * Funciona de manera similar a CComponent_Audio_Source::PlayOneShot(), pero en este caso se especificará la posición en la que se escuchará el sonido.
     * @param x Componente x de la posición en la que reproducirá el sonido.
     * @param y Componente y de la posición en la que reproducirá el sonido.
     * @param z Componente z de la posición en la que reproducirá el sonido.
     *
     * @see CComponent_Audio_Source::PlayOneShot()
     */
    void PlayOneShotAt(ALfloat x, ALfloat y, ALfloat z);

    /**
     * Para el sonido.
     *
     * El sonido se parará.
     *
     * @warning Solo servirá si el sonido ha sido reproducido con CComponent_Audio_Source::Play().
     */
    void Stop();
    /**
     * Pausa el sonido.
     *
     * El sonido será pausado.
     *
     * @warning Solo servirá si el sonido ha sido reproducido con CComponent_Audio_Source::Play().
     */
    void Pause();
    /**
     * Reanuda el sonido.
     *
     * El sonido será reanudado.
     *
     * @warning Solo servirá si el sonido ha sido reproducido con CComponent_Audio_Source::Play().
     */
    void Resume();
    /**
     * Rebobina el sonido.
     *
     * El sonido será rebobinado.
     *
     * @warning Solo servirá si el sonido ha sido reproducido con CComponent_Audio_Source::Play().
     */
    void Rewind();

    /**
     * Reservar un recurso para reproducir un sonido.
     *
     * Se reservará un "source" de OpenAL para poder reproducir un sonido conforme al avance del juego, viéndose modificado por el cambio de frecuencia, de volumen, etc.
     *
     * @warning Debe usarse para usar CComponent_Audio_Source::Play(), y deberá bindearse el sonido cada vez que se cargue un nuevo sonido (al iniciar el objeto, básicamente).
     * @warning Todo recurso bindeado, si se destruye el componente que lo tiene bindeado, será des-bindeado de manera automática con CComponent_Audio_Source::UnBind().
     * @warning Existe un límite de recursos definidos por la variable entera de juego __SOUND_NUMBER_SOURCES
     */
    void Bind();

    /**
     * Liberar un recurso.
     *
     * Se liberará un "source" de OpenAL para dejar espacio para otros sonidos que necesiten recursos.
     *
     * @warning Se deberá llamar de manera manual si se quiere liberar un recurso.
     * @warning Todo recurso bindeado, si se destruye el componente que lo tiene bindeado, será des-bindeado de manera automática con CComponent_Audio_Source::UnBind().
     */
    void UnBind();

  protected:

    void OnLoop();
    /**
     * @brief Renderizar el componente.
     *
     * Si la variable del sistema __RENDER_SOUND_RADIUS dentro del juego es mayor que 1,
     * se dibujarán esferas alrededor de los componentes para representar max_distance y min_distance.
     *
     * @see CComponent::OnLoop().
     *
     * @param projMatrix
     * @param modelViewMatrix
     */
    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
};

/*@}*/

#endif /* __COMPONENT_AUDIO_H_ */
