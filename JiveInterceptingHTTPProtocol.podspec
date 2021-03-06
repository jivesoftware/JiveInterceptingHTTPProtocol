Pod::Spec.new do |s|
  s.name = 'JiveInterceptingHTTPProtocol'
  s.version = '0.1.2'
  s.license = { :type => 'BSD', :file => 'LICENSE' }
  s.summary = 'JiveInterceptingHTTPProtocol is an easy way to intercept NSURLRequests'
  s.homepage = 'https://github.com/jivesoftware/JiveInterceptingHTTPProtocol'
  s.social_media_url = 'http://twitter.com/JiveSoftware'
  s.authors = { 'Jive Mobile' => 'jive-mobile@jivesoftware.com' }
  s.source = { :git => 'https://github.com/jivesoftware/JiveInterceptingHTTPProtocol.git', :tag => s.version }

  s.ios.deployment_target = '7.0'

  s.requires_arc = true
  s.source_files = 'Source/JiveInterceptingHTTPProtocol/*.{h,m}'

end
