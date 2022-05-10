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
	/** @brief Az applikációt futtató osztály. */
	class Sandbox
	{
	public:
		/** @brief A síkidomokat ilyen típusú lambdával kell regisztrálni. */
		using LambdaType = std::function<ConvexShape* (const glib::string&)>;

		using CmdReg = glib::registry<glib::string, const Commands::Command* const>;
		using ShapeReg = glib::registry<glib::string, LambdaType>;
		using ShapeList = glib::list<ConvexShape*>;
	public:
		/*
		* @returns Lambda, amely létrehoz a sablon paraméterként kapott síkidom típusból egy dinamikusan foglalt példányt.
		*/
		template<typename Shape>
		static LambdaType lambdaMaker() {
			return [](const glib::string& type) { return new Shape(type); };
		}

	public:
		Sandbox();
		~Sandbox();

		/** @brief Futtatja az appplikációt. */
		void run();
		/** @brief Leállítja az applikációt. */
		void stop();

		/** @brief Megnyitja a grafikus megjelenítéshez szükséges ablakot. */
		void openWindow();
		/** @brief Bezárja ...*/
		void closeWindow();

		/** @returns A parancs regisztrációs listával */
		CmdReg& getCmdReg();
		/** @returns A síkidom regisztrációs listával */
		ShapeReg& getShapeReg();
		/** @returns A létrehozott síkidomok listájával */
		ShapeList& getShapeList();

		/** @returns A legutóbbi mentés vagy betöltés alkalmával megadott fájl elérési útja */
		const glib::string& getMyFile() const;
		/*
		* @brief Ha a megadott fájl nem üres string, akkor beállítja azt az automatikus mentés helyének. 
		* @param path - A fájl elérési útja
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
	* @brief Automatikusan meghívódik a program kezdetekor.
	* A felhasználó maga definiálja, így a sandboxot egyedi alapállapotba állíthatja,
	* vagy új parancsokat, síkidomokat regisztrálhat.
	*/
	extern void sandboxInit(Sandbox& sandbox);
}
