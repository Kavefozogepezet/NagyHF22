#pragma once

#include <functional>
#include "sfmlinclude.h"

#include "Shapes/Shapes.h"
#include "Commands/Commands.h"
#include "general/list.h"
#include "general/registry.h"
#include "graphics/displays/ConsoleDisplay.h"
#include "graphics/displays/WindowDisplay.h"

namespace CollSys {
	/** @brief Az applik�ci�t futtat� oszt�ly. */
	class Sandbox
	{
	public:
		/** @brief A s�kidomokat ilyen t�pus� lambd�val kell regisztr�lni. */
		using LambdaType = std::function<ConvexShape* (const glib::string&)>;

		using CmdReg = glib::registry<glib::string, const Commands::Command* const>;
		using ShapeReg = glib::registry<glib::string, LambdaType>;
		using ShapeList = glib::list<ConvexShape*>;
	public:
		/*
		* @returns Lambda, amely l�trehoz a sablon param�terk�nt kapott s�kidom t�pusb�l egy dinamikusan foglalt p�ld�nyt.
		*/
		template<typename Shape>
		static LambdaType lambdaMaker() {
			return [](const glib::string& type) { return new Shape(type); };
		}

	public:
		Sandbox();
		~Sandbox();

		/** @brief Futtatja az appplik�ci�t. */
		void run();
		/** @brief Le�ll�tja az applik�ci�t. */
		void stop();

		/** @brief Megnyitja a grafikus megjelen�t�shez sz�ks�ges ablakot. */
		void openWindow();
		/** @brief Bez�rja ...*/
		void closeWindow();

		/** @returns A parancs regisztr�ci�s list�val */
		CmdReg& getCmdReg();
		/** @returns A s�kidom regisztr�ci�s list�val */
		ShapeReg& getShapeReg();
		/** @returns A l�trehozott s�kidomok list�j�val */
		ShapeList& getShapeList();

		/** @returns A legut�bbi ment�s vagy bet�lt�s alkalm�val megadott f�jl el�r�si �tja */
		const glib::string& getMyFile() const;
		/*
		* @brief Ha a megadott f�jl nem �res string, akkor be�ll�tja azt az automatikus ment�s hely�nek. 
		* @param path - A f�jl el�r�si �tja
		*/
		void setMyFile(const glib::string& path);
	private:
		CmdReg cmd_registry;
		ShapeReg s_registry;
		ShapeList shapes;

		ConsoleDisplay cdisp;
		WindowDisplay wdisp;
		
		bool is_running;

		glib::string myfile;
	};

	int csMain(int argc, char** argv);

	/*
	* @brief Automatikusan megh�v�dik a program kezdetekor.
	* A felhaszn�l� maga defini�lja, �gy a sandboxot egyedi alap�llapotba �ll�thatja,
	* vagy �j parancsokat, s�kidomokat regisztr�lhat.
	*/
	extern void sandboxInit(Sandbox& sandbox);
}
