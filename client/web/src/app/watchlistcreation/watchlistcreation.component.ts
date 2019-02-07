import { Component, OnInit } from '@angular/core';
import { symbols } from '../symbols_record';
import { WatchlistService } from '../watchlist.service'
import { watchlist } from '../watchlist';
import {v4 as uuid} from 'uuid';

@Component({
  selector: 'app-watchlistcreation'   ,
  templateUrl: './watchlistcreation.component.html',
  styleUrls: ['./watchlistcreation.component.sass']
})
export class WatchlistcreationComponent implements OnInit {
  symbols_data = symbols;
  watch_list_name: HTMLInputElement;
  watch_list_values: HTMLInputElement;

  constructor(private watchlistservice: WatchlistService) { }

  ngOnInit() {
    this.watch_list_name = document.querySelector('.watchlistcreatestockinputname');
    this.watch_list_values = document.querySelector('.watchlistcreatestockselectstocks');
  }

  create_watchlist(): void{

    var watchlist_obj = {
      "id": uuid(),
      "name": this.watch_list_name.value,
      "stocks": this.get_stock_values(this.watch_list_values)
    }

    this.watchlistservice.new_watchlist(new watchlist(watchlist_obj)).subscribe();

    this.watch_list_name.value = ""
  } 


  get_stock_values(select) {
    var stocks = [];
    var options = select && select.options;

    for (let option of options) {
      if (option.selected) {
        stocks.push(option.value);
      }
    }

    return stocks;
  }

}
