#pragma once

#include <QColor>
#include <QObject>
#include <QPoint>

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

	static constexpr QPoint defaultSceneMin = { -5, -5 };
	static constexpr QPoint defaultSceneMax = { 5, 5 };
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

	// Map action
	static int mapActionDepth();
	static int mapActionPenWidth();
	static Zoom mapActionZoom();
	static void setMapActionDepth(int depth);
	static void setMapActionPenWidth(int penWidth);
	static void setMapActionZoom(Zoom zoom);

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

	// Optimization
	static bool generationOptimized();
	static void setGenerationOptimized(bool enabled);

private:
	Preferences() = default;
	virtual ~Preferences() = default;
};
} // namespace eno
