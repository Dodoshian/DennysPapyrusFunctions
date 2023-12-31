#include "logger.h"

using namespace std;

namespace logger = SKSE::log;

using namespace RE;


/*
Takes a Weapon object as argument.
Returns its weapon template (the weapon it uses as a base in Creation Kit). 

If there is no template then it returns None.
*/
RE::TESObjectWEAP *GetTemplateWeapon(RE::StaticFunctionTag *, RE::TESObjectWEAP *baseWeapon) {
     //const char *name = baseWeapon->GetName();

    // logger::info("Running Function");
    // logger::info("source Weapon is {}", name);
      RE::TESObjectWEAP *templateWeapon = baseWeapon->templateWeapon;
     if (!templateWeapon) {
        // templateWeapon = baseWeapon;
         templateWeapon = nullptr;
     }
      return templateWeapon;
   // TESForm *weep = RE::TESForm::LookupByID(0x00012EB7);
  //  TESObjectWEAP *toReturn = static_cast<TESObjectWEAP *>(weep);
  //  return toReturn;
}


/*
Takes a Armor object as argument.
Returns its Armor template (the Armor it uses as a base in Creation Kit).

If there is no template then it returns None.
*/
RE::TESObjectARMO *GetTemplateArmor(RE::StaticFunctionTag *, RE::TESObjectARMO *baseArmor) {
     // const char *name = baseArmor->GetName();

      // logger::info("Running Function");
     // logger::info("source Armor is {}", name);
      RE::TESObjectARMO *templateArmor = baseArmor->templateArmor;
      if (!templateArmor) {
        // templateArmor = baseArmor;
         templateArmor = nullptr;
      }
      return templateArmor;
}

/*Returns the tempering recipe of a Weapon, if it exists. 
* If the weapon is based on a template weapon, then that weapon is checked for a recipe and it is returned.

Otherwise returns nullptr.*/
RE::BGSConstructibleObject *GetWeaponTemperingRecipe(RE::StaticFunctionTag *, RE::TESObjectWEAP *baseWeapon) { 
    logger::info("running code");
     RE::TESForm *temperKeyword = TESForm::LookupByEditorID("CraftingSmithingSharpeningWheel");
     RE::TESObjectWEAP *tWeapon = baseWeapon->templateWeapon;
      if (!tWeapon) {
         tWeapon = baseWeapon;
      }
      
      RE::BSTArray<RE::BGSConstructibleObject*> thisArray = RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>();
      
      int i = 0;
      for (BGSConstructibleObject* item : thisArray)
      {
        // logger::info("Current item: {}", i);
         i += 1;
         if (item->createdItem == tWeapon) //if this recipe creates the same item we have here...
         {
             if (item->benchKeyword == temperKeyword)  // and the keyword is grindstone
             {
         //        logger::info("This is the recipe for {}", tWeapon->GetName());
                 return item;
             }
         }
           
      }
      return nullptr;
       

}

/*Returns the tempering recipe of an Armor, if it exists.
* If the Armor is based on a template armor, then that armor is checked for a recipe and it is returned.

Otherwise returns nullptr.*/
RE::BGSConstructibleObject *GetArmorTemperingRecipe(RE::StaticFunctionTag *, RE::TESObjectARMO *baseArmor) {
     // logger::info("running code");
      RE::TESForm *temperKeyword = TESForm::LookupByEditorID("CraftingSmithingArmorTable");
      RE::TESObjectARMO *tArmor = baseArmor->templateArmor;
      if (!tArmor) {
         tArmor = baseArmor;
      }

      RE::BSTArray<RE::BGSConstructibleObject *> thisArray =
          RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>();

      int i = 0;
      for (BGSConstructibleObject *item : thisArray) {
       //  logger::info("Current item: {}", i);
         i += 1;
         if (item->createdItem == tArmor)  // if this recipe creates the same item we have here...
         {
             if (item->benchKeyword == temperKeyword)  // and the keyword is armor bench
             {
         //        logger::info("This is the recipe for {}", tArmor->GetName());
                 return item;
             }
         }
      }
      return nullptr;
}

/*
Returns the Recipe required to make this item at the forge, if it exists..

Note this will not work for weapons and armor that use template weapons or armors.
Make sure you use GetTemplateArmor() or GetTemplateWeapon() first in those cases.

By Default, this will check any workbench's recipes. 
To check a specific workbench, pass its keyword in as a string as argument 2.

*/
RE::BGSConstructibleObject *GetCraftingRecipe(RE::StaticFunctionTag *, RE::TESForm *baseForm, string keywordString) 
{
     // logger::info("running code");
      RE::TESForm *temperKeyword = TESForm::LookupByEditorID(keywordString);

      RE::BSTArray<RE::BGSConstructibleObject *> thisArray =
          RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>();
      size_t len = keywordString.length();
      int i = 0;

      if (len == 0)  // if arg 2 is empty string
      {
         for (BGSConstructibleObject *item : thisArray) {
             // logger::info("Current item: {}", i);
             i += 1;
             if (item->createdItem == baseForm)  // if this recipe creates the same item we have here...
             {
                     return item;
             }
         }
      }
      else
      {
         for (BGSConstructibleObject *item : thisArray) {
             // logger::info("Current item: {}", i);
             i += 1;
             if (item->createdItem == baseForm)  // if this recipe creates the same item we have here...
             {
                 if (item->benchKeyword == temperKeyword)  // and the keyword is forge
                 {
                     return item;
                 }
             }
         }
      }

      return nullptr;//return none if this search failed
}

/*returns an array of all recipes that create this item and use the associated keyword.
If keyword string is empty string then it will ignore keywords and return all recipes.
*/
RE::BSTArray<RE::BGSConstructibleObject*> GetCraftingRecipeArray(RE::StaticFunctionTag *, RE::TESForm *baseForm, string keywordString) {
   //   logger::info("running code");
      RE::TESForm *temperKeyword = TESForm::LookupByEditorID(keywordString);

      //holds all COBJs loaded.
      RE::BSTArray<RE::BGSConstructibleObject *> thisArray =
          RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>();

      //this array holds values that will be returned
      RE::BSTArray<RE::BGSConstructibleObject *> returnArray;  // = new RE::BSTArray<RE::BGSConstructibleObject*>;
      
      size_t len = keywordString.length();
      int i = 0;
      int j = 0;

      if (len == 0)//if arg 2 is empty string
      {
         for (BGSConstructibleObject *item : thisArray) {
             i += 1;
             if (item->createdItem == baseForm)  // if this recipe creates the same item we have here...
             {
                j += 1;
                returnArray.resize(j);
                returnArray[j - 1] = item;   
             }
         }
      }
      else//otherwise its a keyword
      {
         for (BGSConstructibleObject *item : thisArray) {
             i += 1;
             if (item->createdItem == baseForm)  // if this recipe creates the same item we have here...
             {
                 if (keywordString.length() == 0 || item->benchKeyword == temperKeyword)  // and the keyword is forge
                 {
                     //     logger::info("This is the recipe for {}", baseForm->GetName());
                     j += 1;
                     returnArray.resize(j);
                     returnArray[j - 1] = item;
                 }
             }
         }
      }

      return returnArray;
}


bool BindPapyrusFunctions(RE::BSScript::IVirtualMachine *vm) {
    vm->RegisterFunction("GetTemplateWeapon", "DennysPapyrusFunctions", GetTemplateWeapon);
    vm->RegisterFunction("GetTemplateArmor", "DennysPapyrusFunctions", GetTemplateArmor);
    vm->RegisterFunction("GetWeaponTemperingRecipe", "DennysPapyrusFunctions", GetWeaponTemperingRecipe);
    vm->RegisterFunction("GetArmorTemperingRecipe", "DennysPapyrusFunctions", GetArmorTemperingRecipe);
    vm->RegisterFunction("GetCraftingRecipe", "DennysPapyrusFunctions", GetCraftingRecipe);
    vm->RegisterFunction("GetCraftingRecipeArray", "DennysPapyrusFunctions", GetCraftingRecipeArray);
     //logger::info("successfully registered function");
    return true;
}


SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SKSE::GetPapyrusInterface()->Register(BindPapyrusFunctions);
   // SetupLog();


    return true;
}
