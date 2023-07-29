class Environment
{
    /*
    Creates environment with given record
    */
   constructor(record ={})
   {
        this.record = record;
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
    if (!this.record.hasOwnProperty(name))
    {
        return new ReferenceError(`Variable "${name}" is not defined`);
    }

    return this.record[name];
   }
}

module.exports = Environment;