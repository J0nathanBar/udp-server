import React, { useEffect, useState } from 'react'
import ReactDOM from "react-dom";
import { Component } from 'react';
import axios from 'axios';

const TextForm = (props) => {
  
  const [msg,setMsg] = useState("")
  const [content,setContent] = useState("")
  const [filename,setFileName] = useState("")

 
  const submits = async () => {

    var obj = {
      attributes: {
       // msg : "value1",
        filename: props.name,
        content: "value1"
    
        
      }
    }
    obj.attributes.filename = filename
    //obj.attributes.msg = msg
    obj.attributes.content = content
   // console.log(filename)
    const a = obj //[obj.attributes.filename, obj.attributes.name]

    const body = { a }
    const response = await fetch('http://localhost:5000/api', {
      method: 'POST',
      headers: { "content-type": "application/json" },
      body: JSON.stringify(body)
    });

    const res = await response.json()
    console.log(res);
  }

  return (
    <div>
      <input type="text"
        placeholder='insert file path'

        value={content}
        
        onChange={(e) => {
          setContent(e.target.value)
        //  setFileName(e.target.name)
      
          setFileName(props.name)
        
        }} />
        

      <button onClick={submits}>submit</button>
    </div>
  )
}
export default TextForm
