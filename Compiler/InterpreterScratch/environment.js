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
}

module.exports = Environment;