import { Component, OnInit } from '@angular/core';
import { symbols } from '../symbols_record';
import { FundamentalsService } from '../fundamentals.service';

@Component({
  selector: 'app-info',
  templateUrl: './info.component.html',
  styleUrls: ['./info.component.sass']
})
export class InfoComponent implements OnInit {
  tickers = symbols;
  selected_ticker: string;
  ticker_data: Array<any> = [];
  ticker_dates: Array<any> = [];
  current_ticker_display: Object = {};
  current_ticker_displayed: boolean = false;

  object_keys = Object.keys;

  constructor(private fundamentalservice: FundamentalsService) { }

  ngOnInit() {
    
  }

  get_ticker_data(ticker_value){
    this.current_ticker_display = {};
    this.current_ticker_displayed = false;
    this.ticker_dates = [];

    this.selected_ticker = ticker_value
    this.fundamentalservice.get_fundamentals(this.selected_ticker).subscribe(response => {
      console.log(response);
      this.ticker_data = response["_embedded"]; 
      this.ticker_dates = [];
      for(let td of this.ticker_data){
          this.ticker_dates.push(td["datafqtr"]);
      }
      console.log(this.ticker_dates)
    });
  }

  display_ticker_data(ticker_quarter){
    this.current_ticker_displayed = true;
    var index = this.ticker_dates.indexOf(ticker_quarter);
    this.current_ticker_display = this.ticker_data[index]
    delete this.current_ticker_display['_id']
  }



}
