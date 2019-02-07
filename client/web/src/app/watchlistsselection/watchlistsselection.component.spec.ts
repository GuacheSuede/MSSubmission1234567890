import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { WatchlistsselectionComponent } from './watchlistsselection.component';

describe('WatchlistsselectionComponent', () => {
  let component: WatchlistsselectionComponent;
  let fixture: ComponentFixture<WatchlistsselectionComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ WatchlistsselectionComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(WatchlistsselectionComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
