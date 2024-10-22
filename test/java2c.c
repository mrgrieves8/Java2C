#include <stddef.h> /* size_t, NULL */
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "object.h"
#include "my_class.h"

#define CAT_COLOR_MAX_LEN 16

static class_t *animal_cls;
static class_t *dog_cls;
static class_t *cat_cls;
static class_t *legendary_animal_cls;

typedef int (*equals_func_ptr)(object_t *, object_t *);
typedef void (*say_hello_ptr)(object_t *);
typedef int (*get_masters_ptr)(object_t *);
typedef void (*to_string_ptr)(object_t *, char *);
typedef void (*finalize_ptr)(object_t *);

/******************************** Animal ********************************/
typedef struct animal_instance_data
{
    int num_legs;
    int num_masters;
    int ID;
} animal_instance_data_t;

void AnimalStaticInitblock(class_t *this)
{
    (void)this;
    printf("Static block Animal 1\n");
    printf("Static block Animal 2\n");
}

void AnimalInstanceInitblock(object_t *this)
{
    (void)this;
    printf("Instance initialization block Animal\n");
}

void AnimalSayHello(object_t *this)
{
    animal_instance_data_t data = {0};
    
    assert(this);

    printf("Animal Hello!\n");
    data = *(animal_instance_data_t *)ObjectGetInstanceData(this, animal_cls);

    printf("I have %d legs\n", data.num_legs);
}

void AnimalShowCounter(void)
{
    int counter = 0;
	
    assert(animal_cls);

	ClassStaticInitBlockExec(animal_cls);
	
    counter = *(int *)ClassGetStaticFieldByName(animal_cls, "counter");
    
    printf("%d\n", counter);

}

void AnimalToString(object_t *this, char *buffer)
{
    animal_instance_data_t data = {0};
    
    assert(this);

    data = *(animal_instance_data_t *)ObjectGetInstanceData(this, animal_cls);

    sprintf(buffer, "Animal with ID: %d", data.ID);
}

void AnimalFinalize(object_t *this)
{
    finalize_ptr parent_finalize = NULL;
    animal_instance_data_t data = {0};

    assert(this);
    assert(animal_cls);

    data = *(animal_instance_data_t *)ObjectGetInstanceData(this, animal_cls);
    parent_finalize = (finalize_ptr)ClassGetMethod(ClassGetParent(animal_cls), "finalize_obj");

    printf("finalize Animal with ID: %d\n", data.ID);

    parent_finalize(this);
}

int AnimalGetNumMasters(object_t *this)
{
    animal_instance_data_t data = {0};
    
    assert(this);

    data = *(animal_instance_data_t *)ObjectGetInstanceData(this, animal_cls);

    return data.num_masters;
}

object_t *Animal(object_t *new_animal)
{
    animal_instance_data_t data = {0};  
    say_hello_ptr say_hello = NULL;
    to_string_ptr my_to_string = NULL;
    to_string_ptr parent_to_string = NULL;
    char buffer[50] = {'\0'};
    int counter = 0;

    assert(animal_cls);

    new_animal = ClassCreateNewInstance(animal_cls, NULL, new_animal);

    /* System.out.println("Animal Ctor"); */
    printf("Animal Ctor\n");

    /* this.ID = ++counter; */
    counter = *(int *)(ClassGetStaticFieldByName(animal_cls, "counter"));
    data = *(animal_instance_data_t *)ObjectGetInstanceData(new_animal, animal_cls);
    data.ID = counter + 1;
    ClassSetStaticFieldByName(animal_cls, "counter", &data.ID);
    ObjectSetInstanceData(new_animal, &data, animal_cls);

    /* sayHello(); */
    say_hello = (say_hello_ptr)ObjectGetMethod(new_animal, "sayHello_obj");
    say_hello(new_animal);

    /* showCounter(); */
    AnimalShowCounter();

    /* System.out.println(toString()); */
    my_to_string = (to_string_ptr)ObjectGetMethod(new_animal, "toString_obj_str");
    my_to_string(new_animal, buffer);
    printf("%s\n", buffer);

    /* System.out.println(super.toString()); */
    parent_to_string = (to_string_ptr)ClassGetMethod(ClassGetParent(animal_cls), "toString_obj_str");
    parent_to_string(new_animal, buffer);
    printf("%s\n", buffer);

    return new_animal;
}

object_t *NewAnimal(void)
{
    object_t *new_animal = ObjectAllocate(animal_cls);

    return Animal(new_animal);
}

object_t *AnimalInt(object_t *new_animal, int num_masters)
{
    animal_instance_data_t data = {0};  
    int counter = 0;

    assert(animal_cls);

    new_animal = ClassCreateNewInstance(animal_cls, NULL, new_animal);

    /* System.out.println("Animal Ctor"); */
    printf("Animal Ctor int\n");

    /* this.ID = ++counter; */
    counter = *(int *)(ClassGetStaticFieldByName(animal_cls, "counter"));
    data = *(animal_instance_data_t *)ObjectGetInstanceData(new_animal, animal_cls);
    data.ID = counter + 1;
    ClassSetStaticFieldByName(animal_cls, "counter", &data.ID);
    
    /* this.num_masters = num_masters;*/
    data.num_masters = num_masters;
    ObjectSetInstanceData(new_animal, &data, animal_cls);

    return new_animal;
}

object_t *NewAnimalInt(int num_masters)
{
    object_t *new_animal = ObjectAllocate(animal_cls);

    return AnimalInt(new_animal, num_masters);
}

/******************************** Dog extends Animal ********************************/

typedef struct dog_instance_data
{
    int num_legs;
} dog_instance_data_t;

void DogStaticInitblock(class_t *this)
{
    (void)this;
    printf("Static block Dog\n");
}

void DogInstanceInitblock(object_t *this)
{
    (void)this;
    printf("Instance initialization block Dog\n");
}

void DogSayHello(object_t *this)
{
    dog_instance_data_t data = {0};
    
    assert(this);

    printf("Dog Hello!\n");
    data = *(dog_instance_data_t *)ObjectGetInstanceData(this, dog_cls);

    printf("I have %d legs\n", data.num_legs);
}

void DogToString(object_t *this, char *buffer)
{
    animal_instance_data_t data = {0};
    
    assert(this);

    data = *(animal_instance_data_t *)ObjectGetInstanceData(this, animal_cls);

    sprintf(buffer, "Dog with ID: %d", data.ID);
}

void DogFinalize(object_t *this)
{
    finalize_ptr parent_finalize = NULL;
    animal_instance_data_t data = {0};

    assert(this);
    assert(dog_cls);

    data = *(animal_instance_data_t *)ObjectGetInstanceData(this, animal_cls);
    parent_finalize = (finalize_ptr)ClassGetMethod(ClassGetParent(dog_cls), "finalize_obj");

    printf("finalize Dog with ID: %d\n", data.ID);

    parent_finalize(this);
}

object_t *CallAnimalInt2(object_t *new_dog)
{
    return AnimalInt(new_dog, 2);
}

object_t *Dog(object_t *new_dog)
{
    assert(dog_cls);

    /* explicit call to create object*/
    new_dog = ClassExtendNewInstance(dog_cls, NULL, CallAnimalInt2, new_dog);

    /* System.out.println("Animal Ctor"); */
    printf("Dog Ctor\n");

    return new_dog;
}

object_t *NewDog(void)
{
    object_t *new_dog = ObjectAllocate(dog_cls);

    return Dog(new_dog);
}

/******************************** Cat extends Animal ********************************/
typedef struct cat_instance_data
{
    char colors[CAT_COLOR_MAX_LEN];
    int num_masters;
} cat_instance_data_t;


void CatStaticInitblock(class_t *this)
{
    (void)this;
    printf("Static block Cat\n");
}

void CatFinalize(object_t *this)
{
    finalize_ptr parent_finalize = NULL;
    animal_instance_data_t data = {0};

    assert(this);
    assert(dog_cls);

    data = *(animal_instance_data_t *)ObjectGetInstanceData(this, animal_cls);
    parent_finalize = (finalize_ptr)ClassGetMethod(ClassGetParent(cat_cls), "finalize_obj");

    printf("finalize Cat with ID: %d\n", data.ID);

    parent_finalize(this);
}

void CatToString(object_t *this, char *buffer)
{
    animal_instance_data_t data = {0};
    
    assert(this);

    data = *(animal_instance_data_t *)ObjectGetInstanceData(this, animal_cls);

    sprintf(buffer, "Cat with ID: %d", data.ID);
}

object_t *CatString(object_t *this, char *colors)
{
    cat_instance_data_t data = {0};

    this = ClassExtendNewInstance(cat_cls, NULL, Animal, this);

    data = *(cat_instance_data_t *)ObjectGetInstanceData(this, cat_cls);
    strncpy(data.colors, colors, CAT_COLOR_MAX_LEN);
    ObjectSetInstanceData(this, &data, cat_cls);

    printf("Cat Ctor with color: %s\n", data.colors);

    return this;
}

object_t *Cat(object_t *this)
{
    cat_instance_data_t data = {0};
    this = CatString(this, "black");

    printf("Cat Ctor\n");

    data = *(cat_instance_data_t *)ObjectGetInstanceData(this, cat_cls);
    data.num_masters = 2;
    ObjectSetInstanceData(this, &data, cat_cls);

    return this;
}

object_t *NewCat(void)
{
    object_t *new_cat = ObjectAllocate(cat_cls);

    return Cat(new_cat);
}

object_t *NewCatString(char *colors)
{
    object_t *new_cat = ObjectAllocate(cat_cls);

    return CatString(new_cat, colors);
}

/******************************** Legendary Animal extends Cat ********************************/

void LegendaryAnimalStaticInitblock(class_t *this)
{
    (void)this;
    printf("Static block Legendary Animal\n");
}

void LegendaryAnimalSayHello(object_t *this)
{
    (void)this;
    printf("Legendary Hello!\n");
}

void LegendaryAnimalFinalize(object_t *this)
{
    finalize_ptr parent_finalize = NULL;
    animal_instance_data_t data = {0};

    assert(this);
    assert(dog_cls);

    data = *(animal_instance_data_t *)ObjectGetInstanceData(this, animal_cls);
    parent_finalize = (finalize_ptr)ClassGetMethod(ClassGetParent(legendary_animal_cls), "finalize_obj");

    printf("finalize Cat with ID: %d\n", data.ID);

    parent_finalize(this);
}

void LegendaryAnimalToString(object_t *this, char *buffer)
{
    animal_instance_data_t data = {0};
    
    assert(this);

    data = *(animal_instance_data_t *)ObjectGetInstanceData(this, animal_cls);

    sprintf(buffer, "LegendaryAnimal with ID: %d", data.ID);
}

object_t *LegendaryAnimal(object_t *new_legend)
{
    new_legend = ClassExtendNewInstance(legendary_animal_cls, NULL, Cat, new_legend);

    printf("Legendary Ctor\n");

    return new_legend;
}

object_t *NewLegendaryAnimal(void)
{
    object_t *new_legend = ObjectAllocate(legendary_animal_cls);

    return LegendaryAnimal(new_legend);
}


/******************************** Class init funcs ********************************/
void InitAnimalClass(void)
{
    int counter = 0;
    animal_instance_data_t animal_default_data = {0};

    animal_cls = ClassCreate(sizeof(animal_instance_data_t), GetObjectClass(), "Animal");

    ClassAddMethod(animal_cls, (func_ptr)AnimalSayHello, "sayHello_obj");
    ClassAddMethod(animal_cls, (func_ptr)AnimalToString, "toString_obj_str");
    ClassAddMethod(animal_cls, (func_ptr)AnimalFinalize, "finalize_obj");
    ClassAddMethod(animal_cls, (func_ptr)AnimalGetNumMasters, "getNumMasters_obj");

    ClassAddNewStaticField(animal_cls, "counter", sizeof(int));
    ClassSetStaticFieldByName(animal_cls, "counter", &counter);

    ClassSetStaticInitBlock(animal_cls, AnimalStaticInitblock);
    
    animal_default_data.num_legs = 5;
    animal_default_data.num_masters = 1;
	ClassSetDefaultInstanceData(animal_cls, &animal_default_data);

    ClassSetInstanceInitBlock(animal_cls, AnimalInstanceInitblock);
}

void InitDogClass(void)
{
    dog_instance_data_t dog_default_data = {0};

    dog_cls = ClassCreate(sizeof(dog_instance_data_t), animal_cls, "Dog");

    ClassAddMethod(dog_cls, (func_ptr)DogSayHello, "sayHello_obj");
    ClassAddMethod(dog_cls, (func_ptr)DogToString, "toString_obj_str");
    ClassAddMethod(dog_cls, (func_ptr)DogFinalize, "finalize_obj");

    ClassSetStaticInitBlock(dog_cls, DogStaticInitblock);
    
    dog_default_data.num_legs = 4;
    ClassSetDefaultInstanceData(dog_cls, &dog_default_data);

    ClassSetInstanceInitBlock(dog_cls, DogInstanceInitblock);
}

void InitCatClass(void)
{
    cat_instance_data_t cat_default_data = {0};

    cat_cls = ClassCreate(sizeof(cat_instance_data_t), animal_cls, "Cat");

    ClassAddMethod(cat_cls, (func_ptr)CatToString, "toString_obj_str");
    ClassAddMethod(cat_cls, (func_ptr)CatFinalize, "finalize_obj");

    ClassSetStaticInitBlock(cat_cls, CatStaticInitblock);
    
    cat_default_data.num_masters = 5;
    ClassSetDefaultInstanceData(cat_cls, &cat_default_data);

}

void InitLegendaryAnimalClass(void)
{
    legendary_animal_cls = ClassCreate(0, cat_cls, "LegendaryAnimal");

    ClassAddMethod(legendary_animal_cls, (func_ptr)LegendaryAnimalSayHello, "sayHello_obj");
    ClassAddMethod(legendary_animal_cls, (func_ptr)LegendaryAnimalToString, "toString_obj_str");
    ClassAddMethod(legendary_animal_cls, (func_ptr)LegendaryAnimalFinalize, "finalize_obj");

    ClassSetStaticInitBlock(legendary_animal_cls, LegendaryAnimalStaticInitblock);
}

/******************************** Main Flow ********************************/

static void foo(object_t *obj)
{
    to_string_ptr to_string = NULL;
    char buffer[50] = {'\0'};

    to_string = (to_string_ptr)ObjectGetMethod(obj, "toString_obj_str");
    to_string(obj, buffer);

    printf("%s\n", buffer);
}

int main(void)
{
    object_t *animal = NULL;
    object_t *dog = NULL;
    object_t *cat = NULL;
    object_t *la = NULL;
    object_t *array[5] = {0};
    say_hello_ptr say_hello = NULL;
    get_masters_ptr get_num_masters = NULL;
    finalize_ptr finalize = NULL;
    size_t i = 0;
    
    /* init classes */
    InitAnimalClass();
    InitDogClass();
    InitCatClass();
    InitLegendaryAnimalClass();

    /* Animal animal = new Animal(); */
    animal = NewAnimal();

    /* Dog dog = new Dog(); */
    dog = NewDog();

    /* Cat cat = new Cat(); */
    cat = NewCat();
    
    /* LegendaryAnimal la = new LegendaryAnimal(); */
    la = NewLegendaryAnimal();

    /* Animal.showCounter(); */
    AnimalShowCounter();

    /*
    System.out.println(animal.ID);
    System.out.println(((Animal)dog).ID);
    System.out.println(((Animal)cat).ID);
    System.out.println(((Animal)la).ID);
    */
    printf("%d\n", (*(animal_instance_data_t *)ObjectGetInstanceData(animal, animal_cls)).ID);
    printf("%d\n", (*(animal_instance_data_t *)ObjectGetInstanceData(dog, animal_cls)).ID);
    printf("%d\n", (*(animal_instance_data_t *)ObjectGetInstanceData(cat, animal_cls)).ID);
    printf("%d\n", (*(animal_instance_data_t *)ObjectGetInstanceData(la, animal_cls)).ID);

    /*
    Animal[] array = {
                new Dog(),
                new Cat(),
                new Cat("white"),
                new LegendaryAnimal(),
                new Animal()
    };
    */
    array[0] = NewDog();
    array[1] = NewCat();
    array[2] = NewCatString("white");
    array[3] = NewLegendaryAnimal();
    array[4] = NewAnimal();

    /* for(Animal a : array) */
    for (i = 0; i < 5; ++i)
    {
        /*a.sayHello();*/
        say_hello = (say_hello_ptr)ObjectGetMethod(array[i], "sayHello_obj");
        say_hello(array[i]);

        /*System.out.println(a.getNumMasters());*/
        get_num_masters = (get_masters_ptr)ObjectGetMethod(array[i], "getNumMasters_obj");
        printf("%d\n", get_num_masters(array[i]));
    }

    for (i = 0; i < 5; ++i)
    {
        foo(array[i]);
    }

    printf("\n\nfinished flow, cleaning up....\n\n\n");

    for (i = 0; i < 5; ++i)
    {
        finalize = (finalize_ptr)ObjectGetMethod(array[i], "finalize_obj");
        finalize(array[i]);
    }

    finalize = (finalize_ptr)ObjectGetMethod(animal, "finalize_obj");
    finalize(animal);

    finalize = (finalize_ptr)ObjectGetMethod(dog, "finalize_obj");
    finalize(dog);

    finalize = (finalize_ptr)ObjectGetMethod(cat, "finalize_obj");
    finalize(cat);

    finalize = (finalize_ptr)ObjectGetMethod(la, "finalize_obj");
    finalize(la);
    
    ClassDestroy(legendary_animal_cls);
    ClassDestroy(dog_cls);
    ClassDestroy(cat_cls);
    ClassDestroy(animal_cls);
    ClassDestroy(GetObjectClass());

	return 0;
}
