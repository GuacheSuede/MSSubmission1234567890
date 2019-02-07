import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { WatchlistsoverviewComponent } from './watchlistsoverview/watchlistsoverview.component';
import { WatchlistcreationComponent } from './watchlistcreation/watchlistcreation.component';
import { AnalyzeComponent } from './analyze/analyze.component'
import { InfoComponent } from './info/info.component'
const routes: Routes = [
  {path: 'watch', component: WatchlistsoverviewComponent},
  {path: 'analyse', component: AnalyzeComponent},
  {path: 'info', component: InfoComponent}
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
