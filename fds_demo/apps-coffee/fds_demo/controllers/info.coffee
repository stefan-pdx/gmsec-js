sc_require 'core'
FdsDemo.infoController = SC.Object.create
   pane: null
   showInfoPane:->
      pane = SC.PanelPane.create
                layout:
                   width: 800
                   height: 500
                   centerX: 0
                   centerY: 0
                contentView: SC.View.extend
                   layout:
                      top: 0
                      left: 0
                      bottom: 0
                      right: 0
                   childViews: 'textView buttonView'.w()
                   textView: SC.TabView.design
                      layout:
                         top: 0
                         bottom: 20
                         left: 0
                         right: 0
                      nowShowing: 'FdsDemo.infoController.pane.overview'
                      items: [{ title: 'Overview',     value: 'FdsDemo.infoController.pane.overview' }
                              { title: 'GMSEC'   ,     value: 'FdsDemo.infoController.pane.gmsec' }
                              { title: 'Technologies', value: 'FdsDemo.infoController.pane.technologies' }]
                      itemTitleKey: 'title'
                      itemValueKey: 'value'
                   buttonView: SC.ButtonView.extend
                      layout:
                         width: 80
                         bottom: 20
                         height: 24
                         centerX: 0
                      title: 'Go Back'
                      action: 'remove'
                      target: 'FdsDemo.infoController.pane'
                overview: SC.LabelView.design
                   escapeHTML: NO
                   layout:
                      left: 0 
                      right: 0
                      top: 20
                      bottom: 0
                   value: '<h3>Welcome to the FDS Demo with real-time web integration!</h3>
                           This web application demonstrates the application of melding together a
                           collection of open-source technologies to help bring the GMSEC API
                           to the web. These technologies include:
                           <ul type="circle">
                             <li><b>CoffeeScript</b>: Used to rapidly write out Javascript for SproutCore.
                             <li><b>Django</b>: Web server that hosts the SproutCore application.
                             <li><b>Git/Github</b>: Version control tool and open-source project hosting website.
                             <li><b>GMSEC</b>: A NASA maintained API to support the development SOA-based applications.
                             <li><b>node.js</b>: The evented Javascript framework that connects to the GMSEC API.
                             <li><b>Protovis</b>: Statistical data visualization package for web browsers.
                             <li><b>Socket.IO</b>: Real-time events between the browser and the node.js server.
                             <li><b>SproutCore</b>: Creates desktop-quality web applications.
                           </ul>
                           <h3>Instructions</h3>'
                gmsec: SC.LabelView.design
                   escapeHTML: NO
                   value: 'GMSEC info here'
                technologies: SC.LabelView.design
                   escapeHTML: NO
                   value: 'Info about technologies go here.'
      pane.append()
      this.set 'pane',pane
