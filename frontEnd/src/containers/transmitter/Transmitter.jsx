import { getValue } from '@testing-library/user-event/dist/utils';
import React from 'react';
import { useRef } from 'react';
import './transmitter.css';
import { TextForm } from '../../components';


const Transmitter = (props) => {
  const inputRef = useRef(null);

  return (
    <div>
      <h1>Transmitter</h1>
      <TextForm></TextForm>
    </div>
    
  )
}


const handler = async() => {
  console.log("eeeeee")
  const a = "eeeeee"
  const body = {a}
  const response = await fetch('http://localhost:5000/send', {
    method: 'POST',
    headers: {"content-type": "application/json"},
    body:body
  });
}

export default Transmitter