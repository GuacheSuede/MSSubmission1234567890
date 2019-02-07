import { Injectable } from '@angular/core';
import {HttpClient, HttpHeaders} from "@angular/common/http";
import {Observable} from "rxjs";

const httpOptions = {
  headers: new HttpHeaders({'Content-Type': 'application/q'})
};


@Injectable({
  providedIn: 'root'
})
export class CompileService {
  compile_url = "http://127.0.0.1:8888";

  constructor(private http: HttpClient) { }

  compile(code: string): Observable<string> {
    return this.http.post<string>(this.compile_url, code);
  }

}
