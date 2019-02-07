export class watchlist {
    id: string;
    name: string;
    stocks: Array<string>;

    constructor(values: Object = {}){
    	Object.assign(this, values)
    }
}
