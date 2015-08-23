{
  'targets': [
    {
      'target_name': 'generate_xwalk_runtime_client_version',
      'type': 'none',
      'actions': [
        {
          # Generate the version for runtime client.
          'action_name': 'generate_runtime_client_version',
          'variables': {
            'template_file': 'app/android/runtime_client/src/templates/XWalkRuntimeClientVersion.template',
            'output_file': '<(SHARED_INTERMEDIATE_DIR)/version_java/XWalkRuntimeClientVersion.java',
          },
          'inputs': [
            'VERSION',
            '<(template_file)',
            'build/android/generate_runtime_client_version.py',
          ],
          'outputs': [
            '<(output_file)',
          ],
          'action': [
            'python', 'build/android/generate_runtime_client_version.py',
            '--template=<(template_file)',
            '--output=<(output_file)',
            '--xwalk-version=<(xwalk_version)',
          ],
        },
      ],
    },
    {
      'target_name': 'xwalk_app_hello_world_apk',
      'type': 'none',
      'dependencies': [
        'xwalk_app_runtime_java',
      ],
      'variables': {
        'apk_name': 'XWalkAppHelloWorld',
        'java_in_dir': 'app/android/app_hello_world',
        'resource_dir': 'app/android/app_hello_world/res',
        'additional_input_paths': [
          '<(PRODUCT_DIR)/app_hello_world/assets/www/index.html',
          '<(PRODUCT_DIR)/app_hello_world/assets/www/sampapp-icon-helloworld.png',
        ],
        'asset_location': '<(PRODUCT_DIR)/app_hello_world/assets',
        'app_manifest_version_code': '<(xwalk_version_code)',
        'app_manifest_version_name': '<(xwalk_version)',
      },
      'copies': [
        {
          'destination': '<(PRODUCT_DIR)/app_hello_world/assets/www',
          'files': [
            'test/android/data/index.html',
            'test/android/data/sampapp-icon-helloworld.png',
          ],
        },
      ],
      'includes': [ '../build/java_apk.gypi' ],
    },
    {
      'target_name': 'xwalk_app_weather_wear_apk',
      'type': 'none',
      'dependencies': [
        'xwalk_app_runtime_java',
      ],
      'variables': {
        'apk_name': 'XWalkAppWeatherWear',
        'java_in_dir': 'app/android/app_weather_wear',
        'resource_dir': 'app/android/app_weather_wear/res',
        'additional_input_paths': [
          '<(PRODUCT_DIR)/app_weather_wear/assets/www/index.html',
          '<(PRODUCT_DIR)/app_weather_wear/assets/www/index_rect.css',
          '<(PRODUCT_DIR)/app_weather_wear/assets/www/index_circle.css',
          '<(PRODUCT_DIR)/app_weather_wear/assets/www/weekly.html',
          '<(PRODUCT_DIR)/app_weather_wear/assets/www/weekly_rect.css',
          '<(PRODUCT_DIR)/app_weather_wear/assets/www/weekly_circle.css',
            '<(PRODUCT_DIR)/app_weather_wear/assets/www/Cloud-Rain-Alt.svg',
            '<(PRODUCT_DIR)/app_weather_wear/assets/www/Cloud-Sun.svg',
            '<(PRODUCT_DIR)/app_weather_wear/assets/www/page_.png',
            '<(PRODUCT_DIR)/app_weather_wear/assets/www/page.png',
           '<(PRODUCT_DIR)/app_weather_wear/assets/www/partlyCloudy_bg.jpg',
           '<(PRODUCT_DIR)/app_weather_wear/assets/www/partly_cloudy.png',
           '<(PRODUCT_DIR)/app_weather_wear/assets/www/Sun.svg',
           '<(PRODUCT_DIR)/app_weather_wear/assets/www/Thermometer-25.svg',
           '<(PRODUCT_DIR)/app_weather_wear/assets/www/Thermometer-75.svg',
           '<(PRODUCT_DIR)/app_weather_wear/assets/www/Umbrella.svg',
           '<(PRODUCT_DIR)/app_weather_wear/assets/www/Wind.svg',

        ],
        'asset_location': '<(PRODUCT_DIR)/app_weather_wear/assets',
        'app_manifest_version_code': '<(xwalk_version_code)',
        'app_manifest_version_name': '<(xwalk_version)',
      },
      'copies': [
        {
          'destination': '<(PRODUCT_DIR)/app_weather_wear/assets/www',
          'files': [
            'test/android/weather_wear/index.html',
            'test/android/weather_wear/index_rect.css',
            'test/android/weather_wear/index_circle.css',
            'test/android/weather_wear/weekly.html',
            'test/android/weather_wear/weekly_rect.css',
            'test/android/weather_wear/weekly_circle.css',
            'test/android/weather_wear/img/Cloud-Rain-Alt.svg',
            'test/android/weather_wear/img/Cloud-Sun.svg',
            'test/android/weather_wear/img/page_.png',
            'test/android/weather_wear/img/page.png',
           'test/android/weather_wear/img/partlyCloudy_bg.jpg',
           'test/android/weather_wear/img/partly_cloudy.png',
           'test/android/weather_wear/img/Sun.svg',
           'test/android/weather_wear/img/Thermometer-25.svg',
           'test/android/weather_wear/img/Thermometer-75.svg',
           'test/android/weather_wear/img/Umbrella.svg',
           'test/android/weather_wear/img/Wind.svg',
         ],
        },
      ],
      'includes': [ '../build/java_apk.gypi' ],
    },
    {
      'target_name': 'xwalk_app_template_apk',
      'type': 'none',
      'dependencies': [
        'xwalk_app_runtime_java',
      ],
      'variables': {
        'apk_name': 'XWalkAppTemplate',
        'java_in_dir': 'app/android/app_template',
        'resource_dir': 'app/android/app_template/res',
        'additional_input_paths': [
          '<(PRODUCT_DIR)/app_template/assets/www/index.html',
          '<(PRODUCT_DIR)/app_template/assets/www/sampapp-icon-helloworld.png',
        ],
        'asset_location': '<(PRODUCT_DIR)/app_template/assets',
      },
      'copies': [
        {
          'destination': '<(PRODUCT_DIR)/app_template/assets/www',
          'files': [
            'test/android/data/index.html',
            'test/android/data/sampapp-icon-helloworld.png',
          ],
        },
      ],
      'includes': [ '../build/java_apk.gypi' ],
    },
    {
      # Combine runtime client and activity into one jar.
      'target_name': 'xwalk_app_runtime_java',
      'type': 'none',
      'dependencies': [
        'generate_xwalk_runtime_client_version',
        'xwalk_core_java',
      ],
      'variables': {
        'java_in_dir': 'app/android/runtime_activity',
        'additional_src_dirs': [
          'app/android/runtime_client',
        ],
        'generated_src_dirs': [ '<(SHARED_INTERMEDIATE_DIR)/version_java' ],
      },
      'includes': ['../build/java.gypi'],
    },
    {
      'target_name': 'prepare_xwalk_app_template',
      'type': 'none',
      'dependencies': [
        'xwalk_app_runtime_java',
        'xwalk_app_template_apk',
        'xwalk_core_library',
        'xwalk_shared_library',
      ],
      'actions': [
        {
          'action_name': 'prepare_xwalk_app_template',
          'message': 'Generating XWalk App Template.',
          'inputs': [
            'build/android/common_function.py',
            'build/android/generate_app_packaging_tool.py',
          ],
          'outputs': [
            '<(PRODUCT_DIR)/prepare_xwalk_app_template_intermediate/always_run',
          ],
          'action': [
            'python', 'build/android/generate_app_packaging_tool.py',
            '<(PRODUCT_DIR)/xwalk_app_template'
          ],
        },
      ],
    },
    {
      'target_name': 'xwalk_app_template',
      'type': 'none',
      'dependencies': [
        'prepare_xwalk_app_template',
      ],
      'actions': [
        {
          'action_name': 'tar_app_template',
          'inputs': [
            'app/android/app_template/AndroidManifest.xml',
            'tools/tar.py',
          ],
          'outputs': [
            '<(PRODUCT_DIR)/xwalk_app_template.tar.gz',
            '<(PRODUCT_DIR)/xwalk_app_template_intermediate/always_run',
          ],
          'action': [
            'python', 'tools/tar.py',
            '<(PRODUCT_DIR)/xwalk_app_template'
          ],
        },
      ],
    },
  ],
}
