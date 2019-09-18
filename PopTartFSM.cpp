#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum state { Out_Of_Poptart, No_Credit, Has_Credit, Dispenses_Poptart };//Storing all States and state paramaters which will be used later.
enum stateParameter { No_Of_Poptarts, Credit, Cost_Of_Poptart };
enum additions { Plain_Base = 1, Spicy_Base = 2, Chocolate_Base = 4, Coconut_Base = 8, Fruity_Base = 16, Chocolate_Filling = 32, Banana_Filling = 64, Strawberry_Filling = 128, Raspberry_Filling = 256, Apple_Filling = 512, Blackberry_Filling = 1024, Maple_Filling = 2048, Marshmallow_Filling = 4096, Cheese_Filling = 8192, CheeseAndHam_Filling = 16384, Caramel_Filling = 32768, Vanilla_Filling = 65536 };//Storing all fillings and bases with option code which will be used later.

class StateContext;//As StateContext is needed before it's actually been created we can declare it above so the the State Class knows that it exists for now and more information will be available later.

class State//All declarations for the State class.
{
protected:
    StateContext* CurrentContext;
    
public:
    State(StateContext* Context)
    { CurrentContext = Context; }
    virtual ~State(void) {}
    virtual void transition(void){}
};

//////////////////////////////////////////////////////////////////////////////////////////

class StateContext//All declarations required for the handling and setting of states.
{
protected:
    
    State* CurrentState = nullptr;
    int stateIndex = 0;
    vector<State*> availableStates;
    vector<int> stateParameters;
    
public://All of the public declarations are virtual as they are getting inherited by other classes and StateContext will be able to switch them at runtime.
    virtual ~StateContext(void);
    virtual void setState(state newState);
    virtual int getStateIndex(void);
    virtual void setStateParam(stateParameter SP, int value);
    virtual int getStateParam(stateParameter SP);
};

StateContext::~StateContext(void)//The state destructor, deletes all states to make the program more secure and allow for memory re-allocation.
{
    for (int i = 0; i < this->availableStates.size(); i++) delete this->availableStates[i];
    this->availableStates.clear();
    this->stateParameters.clear();
}

void StateContext::setState(state newState)//Retrives the state from all current available states and trasnsitions from the current state to the next logical available state.
{
    this->CurrentState = availableStates[newState];
    this->stateIndex = newState;
    this->CurrentState->transition();
}

int StateContext::getStateIndex(void)
{ return this->stateIndex; }

void StateContext::setStateParam(stateParameter SP, int value)//Takes in a state paramater and a value and assigns the value to said chosen paramater.
{ this->stateParameters[SP] = value; }

int StateContext::getStateParam(stateParameter SP)
{ return this->stateParameters[SP]; }

//////////////////////////////////////////////////////////////////////////////////////////

class Transition//A class containing all tranitions available to be used by other classes and a generic error message in case anything unexpected occurs.
{
public:
    virtual bool insertMoney(int) { cout << "Error!" << endl; return false; }
    virtual bool makeSelection(int){ cout << "Error!" << endl; return false; }
    virtual bool moneyRejected(void){ cout << "Error!" << endl; return false; }
    virtual bool addPoptart(int){ cout << "Error!" << endl; return false; }
    virtual bool dispense(void){ cout << "Error!" << endl; return false; }
};

//////////////////////////////////////////////////////////////////////////////////////////

class PoptartState : public State, public Transition
{
public:
    PoptartState(StateContext* Context) : State(Context){}
};

//////////////////////////////////////////////////////////////////////////////////////////

class Product//A class contianing all definitions to create and use a product.
{
    friend class Filling;
protected:
    string product_description = "";
    int itemCost = 0;
    //    virtual Product* ReturnNext(void);
    //    virtual void RemoveHighestCostItem(Product* HighestItem);
public:
    virtual void consume(void);
    virtual int cost(void);
    virtual string description(void);

    //    virtual Product* ReturnHighestCostItem(void);
    //    virtual void RemoveHighestCostItem(void);
};

int Product::cost(void)//Retrieves the item cost which is set by the product or any other class inherting from the Product class.
{ return this->itemCost; }

void Product::consume(void)//Acts as a destructor to delete all data related to a finished product thus allowing for memory re allocation.
{ cout << "Wow Tasty! " << endl; delete this; }

string Product::description(void)//Retrieves the description of a specified product which is set by the product or any other class inherting from the Product class.
{ return this->product_description; }

class Poptart : public Product //The default poptart inherits from the product class.
{
public:
    Poptart(void)
    {
        this->product_description = "Poptart";//Generic string description and item cost are set here.
        this->itemCost = 50;
    }
};

class PlainPoptart : public Poptart//All bases inherit from the Poptart class, this adds to the reusablilty if the code needs to be updated to contain another item(s), i.e drinks we don't want them to be able to interact with the bases.
{
public:
    PlainPoptart(void)
    {
        this->product_description = "Plain Poptart base";
        this->itemCost = 100;
    }
};

class SpicyPoptart : public Poptart
{
public:
    SpicyPoptart(void)
    {
        this->product_description = "Spicy Poptart base";
        this->itemCost = 150;
    }
};

class ChocolatePoptart : public Poptart
{
public:
    ChocolatePoptart(void)
    {
        this->product_description = "Chocolate poptart base";
        this->itemCost = 200;
    }
};

class CoconutPoptart : public Poptart
{
public:
    CoconutPoptart(void)
    {
        this->product_description = "Coconut poptart base";
        this->itemCost = 200;
    }
};

class FruityPoptart : public Poptart
{
public:
    FruityPoptart(void)
    {
        this->product_description = "Fruity poptart base";
        this->itemCost = 200;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////

class Filling : public Product //The filling class is a friend class of product meaning that it can access the private/protected members of the class but we also require it to inherit from the public members for the use of the functions.
{
protected:
    Product* product;
public:
    string description(void);
    int cost(void);
    void addToProduct(Product* newProduct);
};

string Filling::description(void)
{ return product->description() +  " with " + this->product_description; }//Adding the current filling onto the already existing product, can be done as many times as is selected.

int Filling::cost(void)
{ return this->itemCost+ product->cost(); }//The cost of a filling is added to the existing product also.

void Filling::addToProduct(Product* newProduct)//Takes a newProduct pointer that is then set to the orignal product, this allows us to add new fillings to our chosen base.
{ product = newProduct; }



class ChocolateFilling : public Filling//All fillings ar the same as the bases due to the use of the decorator pattern, so each has a description and cost.
{
public:
    ChocolateFilling(void)
    {
        product_description = "Choco filling";
        itemCost = 20;
    }
    
};

class BananaFilling : public Filling
{
public:
    BananaFilling(void)
    {
        product_description = "Banana filling";
        itemCost= 50;
    }
};

class StrawberryFilling : public Filling
{
public:
    StrawberryFilling(void)
    {
        product_description = "Strawberry filling";
        itemCost = 50;
    }
};

class RaspberryFilling : public Filling
{
public:
    RaspberryFilling(void)
    {
        product_description = "Raspberry filling";
        itemCost = 50;
    }
};

class AppleFilling : public Filling
{
public:
    AppleFilling(void)
    {
        product_description = "Apple filling";
        itemCost = 50;
    }
};

class BlackberryFilling : public Filling
{
public:
    BlackberryFilling(void)
    {
        product_description = "Blackberry filling";
        itemCost = 50;
    }
};

class MapleFilling : public Filling
{
public:
    MapleFilling(void)
    {
        product_description = "Maple filling";
        itemCost = 100;
    }
};

class MarshmallowFilling : public Filling
{
public:
    MarshmallowFilling(void)
    {
        product_description = "Marshmallow filling";
        itemCost = 20;
    }
};

class CheeseFilling : public Filling
{
public:
    CheeseFilling(void)
    {
        product_description = "Cheese filling";
        itemCost = 70;
    }
};

class CheeseAndHamFilling : public Filling
{
public:
    CheeseAndHamFilling(void)
    {
        product_description = "Cheese and ham filling";
        itemCost = 100;
    }
};

class CaramelFilling : public Filling
{
public:
    CaramelFilling(void)
    {
        product_description = "Caramel filling";
        itemCost = 20;
    }
};

class VanillaFilling : public Filling
{
public:
    VanillaFilling(void)
    {
        product_description = "Vanilla filling";
        itemCost = 50;
    }
};

//////////////////////////////////////////////////////////////////////////////////////////

class Poptart_Dispenser: public StateContext, public Transition//All of the declarations for the Poptart_Dispenser class.
{
    friend class DispensesPoptart;
    friend class HasCredit;
private:
    PoptartState* PoptartCurrentState = nullptr;
    bool itemDispensed = false;
    //indicates whether a product is there to be retrieved
    Product* DispensedItem = nullptr;
    bool itemRetrieved = false; //indicates whether a product has been retrieved
public:
    Poptart_Dispenser(int inventory_count);
    ~Poptart_Dispenser(void);
    bool insertMoney(int money);
    bool makeSelection(int option);
    bool moneyRejected(void);
    bool addPoptart(int number);
    bool dispense(void);
    Product* getProduct(void);
    virtual void setStateParam(stateParameter SP, int value);//The getters and setters are virtual as the paramaters and states are going to change at runtime.
    virtual int getStateParam(stateParameter SP);
};

//////////////////////////////////////////////////////////////////////////////////////////

class OutOfPoptart: public PoptartState//Each state has all transitions available even if they don't use them, this is for the purposes of inheritance and allows the correct state and state change to be chosen at runtime, any illogical changes(no corresponding arrows on the diagram) are not allowed. If an action is succesfully performed a true boolean value is returned if not we return false.
{
public:
    OutOfPoptart(StateContext* Context) : PoptartState(Context){}
    bool insertMoney(int money);//Invalid operation.
    bool makeSelection(int option);//Invalid operation.
    bool moneyRejected(void);//Rejects money and sets the CREDIT enum to 0.
    bool addPoptart(int number);//Allowed and sets the NO_OF_POPTARTS enum to the number passed in.
    bool dispense(void);//Invalid operation.
};

bool OutOfPoptart::insertMoney(int money)//An example of an illogical state change, as this state change is not shown on the diagram we cannot allow it to happen so we output a generic error message and return a corresponding false boolean.
{
    cout << "ERROR unable to insert money in current state.." << endl;
    return false;
}

bool OutOfPoptart::makeSelection(int option)
{
    cout << "ERROR cannot currently make a selection.." << endl;
    return false;
}

bool OutOfPoptart::moneyRejected(void)//This is a state change that is shown on the diagram but the arrow/action returns to the same state, so for this the appropriate action is performed but we do not call a state change as this is unnecessary
{
    cout << "Your money has been rejected, ejecting.." << endl;
    this->CurrentContext->setStateParam(Credit, 0);
    return true;
}

bool OutOfPoptart::addPoptart(int number)//This is a complete state change, the state diagram showed this as an arrow therefore it is required, we first "insert" a number of poptarts into the machine using the 'number' int and setting it to the NO_OF_POPTARTS state paramater. It then outputs a simple message and performs the appropriate state change wich is from Out_Of_Poptarts to No_Credit in this case, and then the function returns true.
{
    this->CurrentContext->setStateParam(No_Of_Poptarts, number);
    cout << "Poptarts added to machine: " << this->CurrentContext->getStateParam(No_Of_Poptarts) << endl;
    this->CurrentContext->setState(No_Credit);
    return true;
}

bool OutOfPoptart::dispense(void)
{
    cout << "ERROR currently unable to dispense any poptarts.." << endl;
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////

class NoCredit : public PoptartState
{
public:
    NoCredit(StateContext* Context) : PoptartState(Context) {}
    bool insertMoney(int money);//Adds 'money' amount to Credit enum.
    bool makeSelection(int option);//Invalid operation.
    bool moneyRejected(void);//Invalid operation.
    bool addPoptart(int number);//Invalid operation.
    bool dispense(void);//Invalid operation.
};

bool NoCredit::insertMoney(int money)//Adds credit to the machine by taking the amount input and adding it to the Credit enum, also outputting the amount inserted and total. It then sets the state to Has_Credit and returns true to signify the action has been performed.
{
    cout << "Inserted: " << money << endl;
    this->CurrentContext->setStateParam(Credit,this->CurrentContext->getStateParam(Credit) + money);
    cout << "Total: " << this->CurrentContext->getStateParam(Credit) << endl;
    this->CurrentContext->setState(Has_Credit);
    return true;
}

bool NoCredit::makeSelection(int option)
{
    cout << "ERROR cannot currently make a selection.." << endl;
    return false;
}

bool NoCredit::moneyRejected(void)
{
    cout << "ERROR no money available.." << endl;
    return false;
}

bool NoCredit::addPoptart(int number)
{
    cout << "ERROR poptarts already in machine.." << endl;
    return false;
}

bool NoCredit::dispense(void)
{
    cout << "ERROR please insert money.." << endl;
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////

class HasCredit : public PoptartState
{
public:
    HasCredit(StateContext* Context) : PoptartState(Context) {}
    bool insertMoney(int money);//Stays in the same state and adds the amount inserted to the Credit enum.
    bool makeSelection(int option);//Takes an option code in and creates a fully customsied end product using a base and fillings.
    bool moneyRejected(void);//Sets the Credit enum to 0 and sets the current state to No_Credit.
    bool addPoptart(int number);//Invalid operation.
    bool dispense(void);//Invalid operation.
};//As the diagram did not specify any rejection based upon not having enough money for a final product, any poptart is allowed to be created and sent to a Dispensed_Item state where it is checked there, a future update to the code could include a price checking system in which a base and toppings are added and price checked, disallowing any additions over the credit limit.

bool HasCredit::insertMoney(int money)//Adding to the credit enum and staying in the same state.
{
    cout << "Inserted: " << money << endl;
    this->CurrentContext->setStateParam(Credit, this->CurrentContext->getStateParam(Credit) + money);
    cout << "Total: " << this->CurrentContext->getStateParam(Credit) << endl;
    return true;
}

bool HasCredit::makeSelection(int option)//As we needed to input a single option code that had to correspond to multiple values (base and filling(s)) I used bitmasking, using this I was able to store as many values as needed within a single value and then running a compare on the option code with bases and fillings which will return true on all items contaied within the option code.
{
    cout << "You selected option code: " << option << endl;
    Product* product = nullptr;//We create a product pointer but do not set it to anything, this creates a generic poptart.
    Filling* currentFilling;//Declaring our filling pointer.
    
    if(option & Plain_Base)//For bases I used if else as only one base was allowed, if the compare is true for any base it will run the code and create a new base object of that type.
    { product = new PlainPoptart; }
    
    else if(option & Spicy_Base)
    { product = new SpicyPoptart; }
    
    else if(option & Chocolate_Base)
    { product = new ChocolatePoptart; }
    
    else if(option & Coconut_Base)
    { product = new CoconutPoptart; }
    
    else if(option & Fruity_Base)
    { product = new FruityPoptart; }
    
    if(option & Chocolate_Filling)//With the fillings a new if statement was used every time as multiple fillings can be added to each base.
    {
        currentFilling = new ChocolateFilling;//A new instance of the filling is created.
        currentFilling->addToProduct(product);//Adding to the product.
        product = currentFilling;//Product is set to currentFilling as the addToProduct function creates a newProduct to wrap the base and filling. This is done for every filling as you can see.
    }
    
    if(option & Banana_Filling)
    {
        currentFilling = new BananaFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    if(option & Strawberry_Filling)
    {
        currentFilling = new StrawberryFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    if(option & Raspberry_Filling)
    {
        currentFilling = new RaspberryFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    if(option & Apple_Filling)
    {
        currentFilling = new AppleFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    if(option & Blackberry_Filling)
    {
        currentFilling = new BlackberryFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    if(option & Maple_Filling)
    {
        currentFilling = new MapleFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    if(option & Marshmallow_Filling)
    {
        currentFilling = new MarshmallowFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    if(option & Cheese_Filling)
    {
        currentFilling = new CheeseFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    if(option & CheeseAndHam_Filling)
    {
        currentFilling = new CheeseAndHamFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    if(option & Caramel_Filling)
    {
        currentFilling = new CaramelFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    if(option & Vanilla_Filling)
    {
        currentFilling = new VanillaFilling;
        currentFilling->addToProduct(product);
        product = currentFilling;
    }
    
    ((Poptart_Dispenser *)this->CurrentContext)->DispensedItem = product;//We set the DispensedItem to the finished product.
    this->CurrentContext->setState(Dispenses_Poptart);//A state change occurs.
    return true;
}

bool HasCredit::moneyRejected(void)//Resetting the Credit amount to 0 and changing to an out of credit state.
{
    cout << "Your money has been rejected.." << endl;
    this->CurrentContext->setStateParam(Credit, 0);
    this->CurrentContext->setState(No_Credit);
    return true;
}

bool HasCredit::addPoptart(int number)
{
    cout << "ERROR poptarts already inserted.." << endl;
    return false;
}

bool HasCredit::dispense(void)
{
    cout << "ERROR please make a selection first.." << endl;
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////

class DispensesPoptart : public PoptartState
{
public:
    DispensesPoptart(StateContext* Context) : PoptartState(Context) {}
    bool insertMoney(int money);//Invalid operation.
    bool makeSelection(int option);//Invalid operation.
    bool moneyRejected(void);//Invalid opertion.
    bool addPoptart(int number);//Invalid operation.
    bool dispense(void);//Changes to a possible 3 states based upon paramaters.
};

bool DispensesPoptart::insertMoney(int money)
{
    cout << "ERROR please dispense first.." << endl;
    return false;
}

bool DispensesPoptart::makeSelection(int option)
{
    cout << "ERROR selection already made.." << endl;
    return false;
}

bool DispensesPoptart::moneyRejected(void)
{
    cout << "ERROR money has been accepted cannot reject.." << endl;
    return false;
}

bool DispensesPoptart::addPoptart(int number)
{
    cout << "ERROR cannot add poptarts in current state.." << endl;
    return false;
}

bool DispensesPoptart::dispense(void)
{
    if(this->CurrentContext->getStateParam(Credit) >= this->CurrentContext->getStateParam(Cost_Of_Poptart))//A simple credit check to see if the selection made can purchased.
    {//If there is enough credit in the machine the general statistics are output as well as the description of the poptart for clarification purposes.
        cout << "*************************************************" << endl;
        cout << "Enough money available to purchase poptart. " << endl;
        cout << "Credit: " << this->CurrentContext->getStateParam(Credit) << endl;
        cout << "Selection Cost: " << this->CurrentContext->getStateParam(Cost_Of_Poptart) << endl;
        cout << "Dispensing: " << ((Poptart_Dispenser *)(this->CurrentContext))->DispensedItem->description() << endl;
        this->CurrentContext->setStateParam(No_Of_Poptarts, this->CurrentContext->getStateParam(No_Of_Poptarts) - 1);//Removing a poptart from No_Of_Poptarts as it is being dispensed.
        this->CurrentContext->setStateParam(Credit, this->CurrentContext->getStateParam(Credit) - this->CurrentContext->getStateParam(Cost_Of_Poptart));//Removing the cost of the poptart from the overall Credit.
        ((Poptart_Dispenser *)this->CurrentContext)->itemDispensed = true;//After all of the actions using the DispenedItem have been completed it is dispensed from the machine to be consumed.
        cout << "New Credit: " << this->CurrentContext->getStateParam(Credit) << endl;
        cout << "Poptarts Available: " << this->CurrentContext->getStateParam(No_Of_Poptarts) << endl;
        cout << "*************************************************" << endl;
    }
    
    else//Outputting how much extra credit was needed to make the original selection.
    {
        cout << "Credit insufficient to make purchase.." << endl;
        cout << this->CurrentContext->getStateParam(Cost_Of_Poptart) - this->CurrentContext->getStateParam(Credit) << " Credits needed for your selection " << endl;
    }
    
    //Once the item has been dispensed there are three possible state changes that can occur:
    if(this->CurrentContext->getStateParam(No_Of_Poptarts) == 0)
    { this->CurrentContext->setState(Out_Of_Poptart); }//If the dispenser is out of poptarts it will change to an Out_Of_Poptart state.
    
    else if (this->CurrentContext->getStateParam(Credit) > 0)
    { this->CurrentContext->setState(Has_Credit); }//If there is still credit in the machine it goes to a Has_Credit state.
    
    else if(this->CurrentContext->getStateParam(Credit) == 0)
    { this->CurrentContext->setState(No_Credit); }//If there is no credit it changes to a No_Credit state.
    
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

Poptart_Dispenser::Poptart_Dispenser(int inventory_count)//Pushing all of the state enums onto a vector, also pushing on values used for Credit and No_Of_Poptarts.
{
    this->availableStates.push_back(new OutOfPoptart(this));
    this->availableStates.push_back(new NoCredit(this));
    this->availableStates.push_back(new HasCredit(this));
    this->availableStates.push_back(new DispensesPoptart(this));
    this->stateParameters.push_back(0);
    this->stateParameters.push_back(0);

    this->setState(Out_Of_Poptart);//The starting state is always set to thd Out_Of_Poptart state.
    if(inventory_count > 0)
    { this->addPoptart(inventory_count); }
}

Poptart_Dispenser::~Poptart_Dispenser(void)//The destructor for the dispenser.
{
    if (!this->itemRetrieved)
    { delete this->DispensedItem; }
}

bool Poptart_Dispenser::insertMoney(int money)//For all of the state changing actions it looks at the CurrentState, if it's possible to perform the action it allocates it the current state where it is dealt with correctly.
{
    PoptartCurrentState = (PoptartState*) this->CurrentState;
    return this->PoptartCurrentState->insertMoney(money);
}

bool Poptart_Dispenser::makeSelection(int option)
{
    PoptartCurrentState = (PoptartState*) this->CurrentState;
    return this->PoptartCurrentState->makeSelection(option);
}

bool Poptart_Dispenser::moneyRejected(void)
{
    PoptartCurrentState = (PoptartState*) this->CurrentState;
    return this->PoptartCurrentState->moneyRejected();
}

bool Poptart_Dispenser::addPoptart(int number)
{
    PoptartCurrentState = (PoptartState*) this->CurrentState;
    return this->PoptartCurrentState->addPoptart(number);
}

bool Poptart_Dispenser::dispense(void)
{
    PoptartCurrentState = (PoptartState*) this->CurrentState;
    return this->PoptartCurrentState->dispense();
}

Product* Poptart_Dispenser::getProduct(void)//Retrives the DispensedItem which is the final product/selection that was just made and paid for.
{
    if (this->itemDispensed)
    {
        this->itemDispensed = false;
        this->itemRetrieved = true;
        return this->DispensedItem;
    }

    return nullptr;
}
void Poptart_Dispenser::setStateParam(stateParameter SP, int value)//Sets the values of the state parameters except for the Cost_Of_Poptart as that is set by our total of product so the Bases/Fillings classes cost.
{
    if (SP == Cost_Of_Poptart)
    { return; }
    this->stateParameters[SP] = value;
}

int Poptart_Dispenser::getStateParam(stateParameter SP)//As with the setting of state parameters it will return the values set to them except for the Cost_Of_Poptart as this returns the cost of the Dispensed item which is the final product.
{
    if (SP == Cost_Of_Poptart)
    {
        if (DispensedItem == nullptr)
        { return 0; }
        return DispensedItem->cost();
    }
    return this->stateParameters[SP];
}

