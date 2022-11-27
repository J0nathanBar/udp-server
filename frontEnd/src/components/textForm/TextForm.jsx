import React, { useEffect, useState } from 'react'
import ReactDOM from "react-dom";
import { Component } from 'react';
import axios from 'axios';

const TextForm = (props) => {
  const [msg,setMsg] = useState("")
  const submits = async () => {

    var obj = {
      attributes:
      {
        msg : "value1"
        
      }
    }
    obj.attributes.msg = msg
    const a = obj.attributes.msg
    const body = { a }
    const response = await fetch('http://localhost:5000/api', {
      method: 'POST',
      headers: { "content-type": "application/json" },
      body: JSON.stringify(body)
    });
    //export NODE_OPTIONS = --openssl - legacy - provider

    const res = await response.json()
    console.log(res);
  }

  return (
    <div>
      <input type="text"
        placeholder='insert string'

        value={msg}
        onChange={(e) => {
          setMsg(e.target.value)
        }} />

      <button onClick={submits}>submit</button>
    </div>
  )
}
export default TextForm