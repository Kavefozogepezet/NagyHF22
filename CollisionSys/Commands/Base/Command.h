#pragma once

#include <sstream>

#include "general/string.h"

namespace CollSys {
	class Sandbox;
	class ConvexShape;
}

namespace CollSys::Commands {
	/*
	* @brief Egy parancs ilyen típusú kivételt dob,
	* ha a végrahajtás során valamilyen hiba lépett fel.
	*/
	class Error
	{
	public:
		/** @param desc - A hiba leírása */
		Error(const glib::string& desc);

		/** A hiba leírását tartalmazó string */
		glib::string desc;

		/** @brief Kiírja a hiba leírását a standard kimenetre,figyelemfelhívó piros színnel. */
		void print();
	};

	// -------------------- alap osztályok --------------------

	/*
	* @brief Absztrakt osztály, amely leszármazottai végrehajtható parancsok.
	* Egy parancsot úgy hajthatunk végre, ha példányosítjuk, majd meghívjuk az execute függvényét.
	*/
	class Command
	{
	public:
		/** @params sandbox - Az a sandbox osztály, amin a parancs a hatását kifejti. */
		Command(Sandbox& sandbox);

		virtual ~Command() = default;

		/*
		* @brief Kiolvassa a kapott stream-ből a parancs paramétereit,
		* majd azok függvényében kifejti hatását.
		* Ha hiba lép fel, Error típusú kivételt dob.
		* A leszármazott osztályokban felülírandó.
		* @param input - A parancs paramétereit tartalmazó stringstream
		*/
		virtual void execute(std::stringstream& input) const = 0;

		/** @brief A streambe írja a parancs leírását és paraméter listáját. */
		friend std::ostream& operator << (std::ostream& stream, const Command& cmd);
	protected:
		void postInputCheck(std::stringstream& input) const;

		/** @brief A sandbox osztály, amelyen a parancs kifejti hatását. */
		Sandbox& reciever;

		glib::string
			/** @brief A parancs leírása. */
			desc,
			/** @brief A parancs paraméterlistája. */
			params;
	};

	/** Absztrakt osztály, amely specializált a síkidomok állapotának változtatására. */
	class TransformCommand : public Command
	{
	public:
		TransformCommand(Sandbox& sandbox);

		virtual ~TransformCommand() = default;
	protected:
		/*
		* @brief Kikeresi a megadott nevű síkidomot.
		* Ha nincs ilyen, Error típusú kivételt dob.
		* @param name - A keresett síkidom neve
		* @returns Találat esetén a síkidomra mutató pointer
		*/
		ConvexShape* getShape(glib::string& name) const;
	};

	/** @ */
	class CreatorCommand : public Command {
	public:
		CreatorCommand(Sandbox& sandbox);

		virtual ~CreatorCommand() = default;
	protected:
		/*
		* @brief Létrehoz egy megadott típusú síkidomot,
		* a sandbox-ba regisztrált lambda meghívásával.
		* Ha nincs ilyen típus regisztrálva, Error típusú hibát dob.
		* @params key - A létrehozni kívánt síkidom típusa
		* @returns A létrehozott síkidomra mutató pointer
		*/
		ConvexShape* createShape(const glib::string& key) const;

		/*
		* @brief Megállapítja, hogy a megadott síkidomnak valóban egyedi neve van-e.
		* Ha nem, Error típusú kivételt dob. Ez esetben törli a paraméterként kapott síkidomot.
		* @param shape - A tesztelendő síkidomra mutató pointer
		*/
		void validateShape(ConvexShape* shape) const;
	};
}
