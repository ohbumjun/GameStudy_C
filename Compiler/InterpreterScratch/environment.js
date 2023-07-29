class Environment
{
    /*
    Creates environment with given record
    */
   constructor(record ={}, parent = null)
   {
        this.record = record;
        this.parent = parent;
   }

    /*
    Creates variable with given name and vlaue
    */
   define (name, value)
   {
    this.record[name] = value;
    return value;
   }

   lookup(name)
   {
    let env = this.resolve(name);
    console.log("%s", name)
    return env.record[name];
   }

   /*
   return specific environment in which a variable is defined,
   or throws if a variable is not defined
   */
  resolve(name)
  {
    if (this.record.hasOwnProperty(name))
    {
        return this;
    }

    if (this.parent == null)
    {
        // global scope 에 있다는 것.
        // 즉, scope chain 에서 마지막 영역에서도 못찾았다는 것이다.
        return new ReferenceError(`Variable "${name}" is not defined`);
    }

    return this.parent.resolve(name);
  }
}

module.exports = Environment;