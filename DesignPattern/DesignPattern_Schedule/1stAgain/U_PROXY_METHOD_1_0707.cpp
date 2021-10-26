/*
Property Proxy
- I want to use getter,setter functions
but with no getter,setter functions defined 
in class... ?? 
*/

// property proxy
template <typename T>
static Property
{
    T value;
    Property(T value)
    {
        *this = value; // call operator = below;
    }

    operator T()
    {
        return value;
    }

    T &operator=(T new_value)
    {
        cout << "AssignMent\n"
             << endl;
        return value = newValue;
    };
}

/*
struct Creature
{
	int strength{10};
	int agility{12};
	// if we define, getter, setter
	// it will look like below
	int getStrenth() const {
		return strength;
	}
	void setStrength(int strength)
	{
		Creature::strenth=strength;
	}
}
*/

struct Creature
{
    // ...
    // Instead of doing like above
    // we can bulid properly proxy
    // that we can subsequently customize
    // to give you certain behaviors
    // which I actually need
    Property<int> strength{10};
    Property<int> agility{5};
}

int
main()
{
    Creature c;
    c.strength = 11;
    int x = c.agility;
    return 0;
}
