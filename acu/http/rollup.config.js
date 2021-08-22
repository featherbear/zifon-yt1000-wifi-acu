import svelte from 'rollup-plugin-svelte';
import commonjs from '@rollup/plugin-commonjs';
import resolve from '@rollup/plugin-node-resolve';
import replace from '@rollup/plugin-replace';
import livereload from 'rollup-plugin-livereload';
import { terser } from 'rollup-plugin-terser';
import sveltePreprocess from 'svelte-preprocess';
import typescript from '@rollup/plugin-typescript';
import css from 'rollup-plugin-css-only';
import path from 'path'
import fs from 'fs'
const production = !process.env.ROLLUP_WATCH;

let replaceMap = {}
let configuratorData = path.join(__dirname, '../wifi-acu', 'src/utils/configurator_data.h');
if (fs.existsSync(configuratorData)) {
	let fileData = fs.readFileSync(configuratorData, 'utf-8');
	/namespace WWW_PATHS.+?{(.*);.+?namespace WWW_PATHS/s.exec(fileData)[1].trim().split(";").map(s => s.trim()).forEach(cLine => {
		let [lhs, assignment] = cLine.split("=").flatMap(toks => toks.trim())
		lhs = lhs.split(" ").map(toks => toks.trim())
		
		let charTokenIdx;
		let matcher;
		if ((charTokenIdx = lhs.indexOf("char")) > -1 && (lhs.length - charTokenIdx > 1) && (matcher = /(.+?)\[.*\]$/.exec(lhs[charTokenIdx + 1]))) {
			replaceMap["global." + matcher[1]] = assignment;
		} else if ((charTokenIdx = lhs.indexOf("char*")) > -1 && (lhs.length - charTokenIdx > 1)) {
			replaceMap["global." + lhs[charTokenIdx]] = assignment;
		}
	})

	console.info("Replacements have been set up for", replaceMap);
}


function serve() {
	let server;

	function toExit() {
		if (server) server.kill(0);
	}

	return {
		writeBundle() {
			if (server) return;
			server = require('child_process').spawn('npm', ['run', 'start', '--', '--dev'], {
				stdio: ['ignore', 'inherit', 'inherit'],
				shell: true
			});

			process.on('SIGTERM', toExit);
			process.on('exit', toExit);
		}
	};
}

export default {
	input: 'src/main.ts',
	output: {
		sourcemap: !production,
		format: 'iife',
		name: 'app',
		file: 'public/www/app/bundle.js'
	},
	plugins: [
		replace({
			preventAssignment: true,
			values: replaceMap
		}),
		svelte({
			preprocess: sveltePreprocess({ sourceMap: !production }),
			compilerOptions: {
				// enable run-time checks when not in production
				dev: !production
			}
		}),
		// we'll extract any component CSS out into
		// a separate file - better for performance
		css({ output: 'bundle.css' }),

		// If you have external dependencies installed from
		// npm, you'll most likely need these plugins. In
		// some cases you'll need additional configuration -
		// consult the documentation for details:
		// https://github.com/rollup/plugins/tree/master/packages/commonjs
		resolve({
			browser: true,
			dedupe: ['svelte']
		}),
		commonjs(),
		typescript({
			sourceMap: !production,
			inlineSources: !production
		}),

		// In dev mode, call `npm run start` once
		// the bundle has been generated
		!production && serve(),

		// Watch the `public` directory and refresh the
		// browser on changes when not in production
		!production && livereload('public'),

		// If we're building for production (npm run build
		// instead of npm run dev), minify
		production && terser()
	],
	watch: {
		clearScreen: false
	}
};
