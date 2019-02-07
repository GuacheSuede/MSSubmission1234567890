import { Component, OnInit, Input } from '@angular/core';
import { IexService } from '../iex.service';
import { map, filter, switchMap } from 'rxjs/operators';
import {interval} from "rxjs/internal/observable/interval";

@Component({
  selector: 'app-stockitem',
  templateUrl: './stockitem.component.html',
  styleUrls: ['./stockitem.component.sass']
})
export class StockitemComponent implements OnInit {
  @Input("ticker") stockitem: string;
  price: string = "0";
  price_state: boolean;

  constructor(private iexservice: IexService) { }

  ngOnInit() {
    
    interval(5000+Math.random()).pipe(switchMap(() =>  this.iexservice.get_price(this.stockitem))).subscribe(price => {
      console.log(price + "====" + this.price);

      if(price > this.price){
        this.price_state = true;
        this.price = price;
        console.log("_--");
      }

      if(price < this.price){
          this.price_state = false;
          this.price = price;
        console.log("-_-");
      }

    });

    
  }

}
