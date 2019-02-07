import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { HttpClientModule }    from '@angular/common/http';
import { AppComponent } from './app.component';

import { WatchlistsoverviewComponent } from './watchlistsoverview/watchlistsoverview.component';
import { WatchlistsselectionComponent } from './watchlistsselection/watchlistsselection.component';
import { WatchlistviewComponent } from './watchlistview/watchlistview.component';
import { WatchlistcreationComponent } from './watchlistcreation/watchlistcreation.component';
import { StockitemComponent } from './stockitem/stockitem.component';
import { AnalyzeComponent } from './analyze/analyze.component';
import { InfoComponent } from './info/info.component';
import {MonacoEditorModule} from 'ng-monaco-editor';
import {FormsModule} from "@angular/forms";



@NgModule({
  declarations: [
    AppComponent,
    WatchlistsoverviewComponent,
    WatchlistsselectionComponent,
    WatchlistviewComponent,
    WatchlistcreationComponent,
    StockitemComponent,
    AnalyzeComponent,
    InfoComponent
    ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    FormsModule,
    MonacoEditorModule.forRoot({
      // Angular CLI currently does not handle assets with hashes. We manage it by manually adding
      // version numbers to force library updates:
      baseUrl: 'lib',
      defaultOptions: {},
    }),
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
