/***************************************************************************
 *   Copyright (C) 2005 by Grup de Gràfics de Girona                       *
 *   http://iiia.udg.es/GGG/index.html?langu=uk                            *
 *                                                                         *
 *   Universitat de Girona                                                 *
 ***************************************************************************/
#ifndef UDGQ3DVIEWER_H
#define UDGQ3DVIEWER_H

#include "qviewer.h"

// FWD declarations
class vtkRenderer;
class vtkImageCast;
class vtkRenderWindowInteractor;
class vtkInteractorStyle;

namespace udg {

/**
Classe base per als visualitzadors 3D

@author Grup de Gràfics de Girona  ( GGG )
*/

// FWD declarations
class Volume;
class Q3DOrientationMarker;
class Q3DViewerToolManager;

class Q3DViewer : public QViewer{
Q_OBJECT
public:
    enum RenderFunction{ RayCasting , MIP3D, IsoSurface , Texture2D , Texture3D, Contouring, MarchingCubes };

    Q3DViewer( QWidget *parent = 0 );
    ~Q3DViewer();

    /// retorna el tipu de visualització que es té assignat
    RenderFunction getRenderFunction() const { return m_renderFunction; }

    /// retorna el tipu de visualització que es té assignat com a un string
    QString getRenderFunctionAsString();

    virtual void setInput( Volume* volume );

    virtual vtkRenderer *getRenderer();

    /// Retorna el vtkInteractorStyle que té associat
    vtkInteractorStyle *getInteractorStyle();

public slots:
    /// assignem el tipus de visualització 3D que volem. RayCasting, MIP, reconstrucció de superfícies...
    void setRenderFunction(RenderFunction function){ m_renderFunction = function; };
    void setRenderFunctionToRayCasting(){ m_renderFunction = RayCasting; };
    void setRenderFunctionToMIP3D(){ m_renderFunction = MIP3D; };
    void setRenderFunctionToIsoSurface(){ m_renderFunction = IsoSurface; };
    void setRenderFunctionToTexture2D(){ m_renderFunction = Texture2D; };
    void setRenderFunctionToTexture3D(){ m_renderFunction = Texture3D; };
    void setRenderFunctionToContouring(){ m_renderFunction = Contouring; };
    void setRenderFunctionToMarchingcubes(){ m_renderFunction = MarchingCubes; };

    /// Reinicia la vista
    void resetViewToAxial();
    void resetViewToSagital();
    void resetViewToCoronal();

    /// mètodes per controlar la visibilitat de l'orientation marker widget
    void enableOrientationMarker( bool enable );
    void orientationMarkerOn();
    void orientationMarkerOff();

    void setEnableTools( bool enable );
    void enableTools();
    void disableTools();

    void setTool( QString toolName );

    virtual void render();
    void reset();
    
    ///mètode per assignar dels paràmetres necessaris per executar marching cubes
    void setMarchingCubesParameters( int iterations, double relaxation, double angle, double convergence );

protected:
    /// el renderer
    vtkRenderer* m_renderer;
    /// la funció que es fa servir pel rendering
    RenderFunction m_renderFunction;

private:
    /// fa la visualització per raycasting
    void renderRayCasting();

    /// fa la visualització per MIP3D
    void renderMIP3D();

    /// fa la visualització per reconstrucció de superfíces
    void renderIsoSurface();

    /// fa la visualització per textures 2D \TODO afegir comprovació de si el hard o suporta o no
    void renderTexture2D();

    /// fa la visualització per textures 3D \TODO afegir comprovació de si el hard o suporta o no
    void renderTexture3D();
    
    /// fa la visualització per contorn de superfíces
    void renderContouring();
    
    /// fa la visualització mitjançant marching cubes
    void renderMarchingCubes();
    
    /// rescala les dades en el format adequat per als corresponents algorismes. Retorna fals si no hi ha cap volum assignat
    bool rescale();

    enum { Axial , Sagital , Coronal };
    /// Canvia la orientació de la càmera
    void setCameraOrientation( int orientation );

    /// el caster de les imatges
    vtkImageCast* m_imageCaster;

    /// Orientació que tenim
    int m_currentOrientation;

    /// Widget per veure la orientació en 3D
    Q3DOrientationMarker *m_orientationMarker;

    /// reinicia la orientació
    void resetOrientation();

    /// El manager de les tools
    Q3DViewerToolManager *m_toolManager;
    
    ///atributs de la renderització marching cubes
    ///nombre d'iteracions que aplica el mètode
    int m_iterations; 
    
    ///relaxació
    double m_relaxation;
    
    ///angle de les normals
    double m_angle; 
    
    ///convergència
    double m_convergence;
};

};  //  end  namespace udg

#endif
