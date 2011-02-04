window.Timeline_ajax_url = static_url 'simile-ajax-api.js'
window.Timeline_urlPrefix = window.Timeline_ajax_url.substring 0,18
window.Timeline_parameters = 'bundle=true'
window.Data_proxy =
                    address: "127.0.0.1"
                    options:
                        port: 8124
                        rememberTransport: true

simileScript = document.createElement 'script'
simileScript.type = 'text/javascript'
simileScript.src = 'simile-ajax-api.js'

head = document.getElementsByTagName('head').item(0);
head.appendChild simileScript
