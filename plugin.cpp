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
     const char *name = baseWeapon->GetName();

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
      const char *name = baseArmor->GetName();

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

By Default, this will only check recipes that use the Forge. 
To check a different workbench, pass its keyword in as a string as argument 2.

*/
RE::BGSConstructibleObject *GetCraftingRecipe(RE::StaticFunctionTag *, RE::TESForm *baseForm, string keywordString) 
{
     // logger::info("running code");
      RE::TESForm *temperKeyword = TESForm::LookupByEditorID(keywordString);

      RE::BSTArray<RE::BGSConstructibleObject *> thisArray =
          RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>();

      int i = 0;
      for (BGSConstructibleObject *item : thisArray) {
         //logger::info("Current item: {}", i);
         i += 1;
         if (item->createdItem == baseForm)  // if this recipe creates the same item we have here...
         {
             if (item->benchKeyword == temperKeyword)  // and the keyword is forge
             {
               //  logger::info("This is the recipe for {}", baseForm->GetName());
                 return item;
             }
         }
      }
      return nullptr;
}

/*returns an array of all recipes that create this item and use the associated keyword*/
RE::BSTArray<RE::BGSConstructibleObject*> GetCraftingRecipeArray(RE::StaticFunctionTag *, RE::TESForm *baseForm, string keywordString) {
   //   logger::info("running code");
      RE::TESForm *temperKeyword = TESForm::LookupByEditorID(keywordString);

      //holds all COBJs loaded.
      RE::BSTArray<RE::BGSConstructibleObject *> thisArray =
          RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSConstructibleObject>();

      //this array holds values that will be returned
      RE::BSTArray<RE::BGSConstructibleObject *> returnArray;  // = new RE::BSTArray<RE::BGSConstructibleObject*>;
      

      int i = 0;
      int j = 0;
      for (BGSConstructibleObject *item : thisArray) {
        // logger::info("Current item: {}", i);
         i += 1;
         if (item->createdItem == baseForm)  // if this recipe creates the same item we have here...
         {
             if (item->benchKeyword == temperKeyword)  // and the keyword is forge
             {
            //     logger::info("This is the recipe for {}", baseForm->GetName());
                 j += 1;
                 returnArray.resize(j);
                 returnArray[j - 1] = item;
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

struct OurEventSink : public RE::BSTEventSink<RE::TESHitEvent>
{
    RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent *event, RE::BSTEventSource <RE::TESHitEvent>* source)
    {
         string targetName = event->target->GetBaseObject()->GetName();
         string attackerName = event->cause->GetBaseObject()->GetName();
         logger::info("{} hit {}", attackerName, targetName);
         return RE::BSEventNotifyControl::kContinue;
    }
};

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SKSE::GetPapyrusInterface()->Register(BindPapyrusFunctions);
    SetupLog();

  //  OurEventSink eventSink = OurEventSink();

    OurEventSink * eventSink = new OurEventSink();

    ScriptEventSourceHolder* holder = RE::ScriptEventSourceHolder::GetSingleton();

    holder->AddEventSink(eventSink);
    //holder->AddEventSink()

    return true;
}


/*
void OnDataLoaded() {
    int formID = 0x64b3d;

    RE::TESForm *form = RE::TESForm::LookupByID(formID);

    

    int value = form->GetGoldValue();
    logger::info("Gold value of sweetroll: {}", value);
    int merID = 0xB9980;

    RE::TESForm *marcurio = RE::TESForm::LookupByID(merID);
    const char *name = marcurio->GetName();
    logger::info("Name: {}", name);
}


void OnMessage(SKSE::MessagingInterface::Message *tMessage) 
{
    int tType = tMessage->type;
    if (tType == SKSE::MessagingInterface::kDataLoaded) {
       // logger::info("about to load a game");
       OnDataLoaded();
    }
        
   // logger::info("Message Type: {}", tType);
}



SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SetupLog();

    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
   //logger::info()
    return true;
}*/