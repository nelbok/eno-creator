#pragma once

#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtGui/QColor>

namespace eno {
class Preferences {
	Q_GADGET

public:
	enum class TypeAction {
		Remove,
		Add,
		Picker,
		Resize,
	};
	Q_ENUM(TypeAction)

	enum class Zoom : int {
		x25 = 25,
		x50 = 50,
		x100 = 100,
		x200 = 200,
	};
	Q_ENUM(Zoom)
	static QString toString(Zoom zoom);

	static constexpr auto minDepth = -100;
	static constexpr auto maxDepth = 100;
	static constexpr auto minPenWidth = 1;
	static constexpr auto maxPenWidth = 50;

public:
	// Scene
	static QPoint sceneMin();
	static QPoint sceneMax();
	static void setSceneMin(const QPoint& min);
	static void setSceneMax(const QPoint& max);

	// Map
	static int mapDepth();
	static int mapPenWidth();
	static Zoom mapZoom();
	static void setMapDepth(int depth);
	static void setMapPenWidth(int penWidth);
	static void setMapZoom(Zoom zoom);

	// Material
	static QString materialName();
	static void setMaterialName(const QString& name);
	static QColor materialDiffuse();
	static void setMaterialDiffuse(const QColor& diffuse);

	// Locations
	static QString projectLocation();
	static QString generateLocation();
	static void setProjectLocation(const QString& location);
	static void setGenerateLocation(const QString& location);

	// Others
	static bool generationOptimized();
	static void setGenerationOptimized(bool enabled);
	static int maximumCommands();
	static void setMaximumCommands(int maximum);

	// Key binding
	static Qt::Key keyRemove();
	static void setKeyRemove(Qt::Key key);

	static Qt::Key keyAdd();
	static void setKeyAdd(Qt::Key key);

	static Qt::Key keyPicker();
	static void setKeyPicker(Qt::Key key);

	static Qt::Key keyResize();
	static void setKeyResize(Qt::Key key);

	static Qt::Key key3DView();
	static void setKey3DView(Qt::Key key);

private:
	Preferences() = default;
	virtual ~Preferences() = default;
};
} // namespace eno
