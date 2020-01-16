function collapseControlCallback (event) {
  let button = $( event.target );
  let str = button.text();
  if (str === 'Show details') {
    button.text('Hide details');
  }
  else {
    button.text('Show details');
  }
  button.siblings( ".collapsible" ).toggle('fast');
}

$( document ).ready( function() {
  // hide all collapsibles
  $( ".collapsible" ).each(function(index,element) {
    $( element ).toggle(0);
  });
  
  // send data to server as sliders are moved
  $( "input[type=range]" ).change( function() {
    let data = {};
    $( 'input[id*="Master"]' ).each(function(index,element) {
      let alpha = Number($(element).val());
      $(element).siblings('div').children('input').each(function(index,element) {
        data[$(element).attr('name')] = alpha*Number($(element).val());
      });
    });
    $.ajax({
      url: '/update',
      method: 'POST',
      dataType: 'html',
      data: data,
    });
    return false;
  });

  // control collapsibles
  $( ".collapseControl" ).click(collapseControlCallback);
});
