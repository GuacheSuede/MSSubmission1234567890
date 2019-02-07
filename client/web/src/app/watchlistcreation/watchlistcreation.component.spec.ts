import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { WatchlistcreationComponent } from './watchlistcreation.component';

describe('WatchlistcreationComponent', () => {
  let component: WatchlistcreationComponent;
  let fixture: ComponentFixture<WatchlistcreationComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ WatchlistcreationComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(WatchlistcreationComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
